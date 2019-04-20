//
//  yas_processing_track_tests.mm
//

#import <XCTest/XCTest.h>
#import <processing/yas_processing_umbrella.h>

using namespace yas;
using namespace yas::proc;

@interface yas_processing_track_tests : XCTestCase

@end

@implementation yas_processing_track_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create_track {
    proc::track track;

    XCTAssertTrue(track);
    XCTAssertEqual(track.modules().size(), 0);
}

- (void)test_create_null {
    proc::track track = nullptr;

    XCTAssertFalse(track);
}

- (void)test_insert_module {
    proc::track track;

    proc::module module1{[] { return proc::module::processors_t{}; }};
    proc::module module2{[] { return proc::module::processors_t{}; }};

    track.insert_module({0, 1}, std::move(module1));
    track.insert_module({1, 1}, std::move(module2));

    XCTAssertEqual(track.modules().size(), 2);

    if (auto const &const_track = track) {
        std::size_t idx = 0;
        for (auto const &pair : const_track.modules()) {
            auto const &time_range = pair.first;
            auto const &module_vec = pair.second;

            XCTAssertEqual(module_vec.size(), 1);

            if (idx == 0) {
                XCTAssertTrue((time_range == proc::time::range{0, 1}));
            } else if (idx == 1) {
                XCTAssertTrue((time_range == proc::time::range{1, 1}));
            } else {
                XCTFail();
            }

            ++idx;
        }
    }
}

- (void)test_remove_module {
    proc::track track;

    proc::module module1{[] { return proc::module::processors_t{}; }};
    proc::module module2{[] { return proc::module::processors_t{}; }};

    track.insert_module({0, 1}, module1);
    track.insert_module({1, 1}, module2);

    XCTAssertEqual(track.modules().size(), 2);

    track.erase_module(module1);

    XCTAssertEqual(track.modules().size(), 1);
    XCTAssertEqual(track.modules().begin()->second.size(), 1);
    XCTAssertEqual(track.modules().begin()->second.at(0), module2);
}

- (void)test_insert_and_erase_modules_same_range {
    proc::track track;

    proc::module module1{[] { return proc::module::processors_t{}; }};
    proc::module module2{[] { return proc::module::processors_t{}; }};

    track.insert_module({0, 1}, module1);

    XCTAssertEqual(track.modules().size(), 1);
    XCTAssertEqual(track.modules().begin()->first, (proc::time::range{0, 1}));
    XCTAssertEqual(track.modules().begin()->second.size(), 1);
    XCTAssertEqual(track.modules().begin()->second.at(0), module1);

    track.insert_module({0, 1}, module2);

    XCTAssertEqual(track.modules().size(), 1);
    XCTAssertEqual(track.modules().begin()->second.size(), 2);
    XCTAssertEqual(track.modules().begin()->second.at(0), module1);
    XCTAssertEqual(track.modules().begin()->second.at(1), module2);

    track.erase_module(module1);

    XCTAssertEqual(track.modules().size(), 1);
    XCTAssertEqual(track.modules().begin()->second.size(), 1);
    XCTAssertEqual(track.modules().begin()->second.at(0), module2);

    track.erase_module(module2);

    XCTAssertEqual(track.modules().size(), 0);
}

- (void)test_total_range {
    proc::track track;

    XCTAssertFalse(track.total_range());

    track.insert_module({0, 1}, proc::module{[] { return proc::module::processors_t{}; }});

    XCTAssertEqual(track.total_range(), (proc::time::range{0, 1}));

    track.insert_module({1, 1}, proc::module{[] { return proc::module::processors_t{}; }});

    XCTAssertEqual(track.total_range(), (proc::time::range{0, 2}));

    track.insert_module({99, 1}, proc::module{[] { return proc::module::processors_t{}; }});

    XCTAssertEqual(track.total_range(), (proc::time::range{0, 100}));

    track.insert_module({-10, 1}, proc::module{[] { return proc::module::processors_t{}; }});

    XCTAssertEqual(track.total_range(), (proc::time::range{-10, 110}));
}

- (void)test_copy {
    std::vector<int> called;

    auto index = std::make_shared<int>(0);
    proc::module module{[index = std::move(index), &called] {
        auto processor = [index = *index, &called](time::range const &, connector_map_t const &,
                                                   connector_map_t const &, stream &) { called.push_back(index); };
        ++(*index);
        return module::processors_t{std::move(processor)};
    }};

    proc::track track;
    track.insert_module({0, 1}, std::move(module));

    auto copied_track = track.copy();

    XCTAssertEqual(copied_track.modules().size(), 1);
    XCTAssertEqual(copied_track.modules().count({0, 1}), 1);

    proc::stream stream{sync_source{1, 1}};

    track.process({0, 1}, stream);

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), 0);

    copied_track.process({0, 1}, stream);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), 1);
}

- (void)test_copy_to_modules {
    proc::module module1{[] { return proc::module::processors_t{}; }};
    proc::module module2{[] { return proc::module::processors_t{}; }};
    proc::module module3{[] { return proc::module::processors_t{}; }};
    proc::module module4{[] { return proc::module::processors_t{}; }};

    proc::track::modules_holder_map_t src_modules;
    src_modules.emplace(proc::time::range{0, 1}, chaining::vector::holder<module>{{module1, module2}});
    src_modules.emplace(proc::time::range{1, 1}, chaining::vector::holder<module>{{module3, module4}});

    auto dst_modules = proc::copy_to_modules(src_modules);

    XCTAssertEqual(src_modules.at(proc::time::range{0, 1}).size(), 2);
    XCTAssertEqual(src_modules.at(proc::time::range{1, 1}).size(), 2);

    XCTAssertEqual(dst_modules.size(), 2);
    XCTAssertEqual(dst_modules.count(proc::time::range{0, 1}), 1);
    XCTAssertEqual(dst_modules.at(proc::time::range{0, 1}).size(), 2);
    XCTAssertEqual(dst_modules.count(proc::time::range{1, 1}), 1);
    XCTAssertEqual(dst_modules.at(proc::time::range{1, 1}).size(), 2);
}

- (void)test_to_modules_holders {
    proc::module module1{[] { return proc::module::processors_t{}; }};
    proc::module module2{[] { return proc::module::processors_t{}; }};
    proc::module module3{[] { return proc::module::processors_t{}; }};
    proc::module module4{[] { return proc::module::processors_t{}; }};

    proc::track::modules_map_t src_modules;
    src_modules.emplace(proc::time::range{0, 1}, module_vector_t{module1, module2});
    src_modules.emplace(proc::time::range{1, 1}, module_vector_t{module3, module4});

    auto dst_modules = proc::to_modules_holders(std::move(src_modules));

    XCTAssertEqual(src_modules.at(proc::time::range{0, 1}).size(), 0);
    XCTAssertEqual(src_modules.at(proc::time::range{1, 1}).size(), 0);

    XCTAssertEqual(dst_modules.size(), 2);
    XCTAssertEqual(dst_modules.count(proc::time::range{0, 1}), 1);
    XCTAssertEqual(dst_modules.at(proc::time::range{0, 1}).size(), 2);
    XCTAssertEqual(dst_modules.at(proc::time::range{0, 1}).at(0), module1);
    XCTAssertEqual(dst_modules.at(proc::time::range{0, 1}).at(1), module2);
    XCTAssertEqual(dst_modules.count(proc::time::range{1, 1}), 1);
    XCTAssertEqual(dst_modules.at(proc::time::range{1, 1}).size(), 2);
    XCTAssertEqual(dst_modules.at(proc::time::range{1, 1}).at(0), module3);
    XCTAssertEqual(dst_modules.at(proc::time::range{1, 1}).at(1), module4);
}

@end
