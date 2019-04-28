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

- (void)test_push_back_module {
    proc::track track;

    proc::module module1{[] { return proc::module::processors_t{}; }};
    proc::module module2{[] { return proc::module::processors_t{}; }};

    track.push_back_module(std::move(module1), {0, 1});
    track.push_back_module(std::move(module2), {1, 1});

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

- (void)test_insert_module {
    proc::track track;

    proc::module module1{[] { return proc::module::processors_t{}; }};
    proc::module module2{[] { return proc::module::processors_t{}; }};
    proc::module module3{[] { return proc::module::processors_t{}; }};

    track.insert_module(module1, 0, {0, 1});
    track.insert_module(module2, 0, {0, 1});
    track.insert_module(module3, 1, {0, 1});

    XCTAssertEqual(track.modules().at({0, 1}).size(), 3);
    XCTAssertEqual(track.modules().at({0, 1}).at(0), module2);
    XCTAssertEqual(track.modules().at({0, 1}).at(1), module3);
    XCTAssertEqual(track.modules().at({0, 1}).at(2), module1);
}

- (void)test_remove_module {
    proc::track track;

    proc::module module1{[] { return proc::module::processors_t{}; }};
    proc::module module2{[] { return proc::module::processors_t{}; }};

    track.push_back_module(module1, {0, 1});
    track.push_back_module(module2, {1, 1});

    XCTAssertEqual(track.modules().size(), 2);

    track.erase_module(module1);

    XCTAssertEqual(track.modules().size(), 1);
    XCTAssertEqual(track.modules().begin()->second.size(), 1);
    XCTAssertEqual(track.modules().begin()->second.at(0), module2);
}

- (void)test_push_back_and_erase_modules_same_range {
    proc::track track;

    proc::module module1{[] { return proc::module::processors_t{}; }};
    proc::module module2{[] { return proc::module::processors_t{}; }};

    track.push_back_module(module1, {0, 1});

    XCTAssertEqual(track.modules().size(), 1);
    XCTAssertEqual(track.modules().begin()->first, (proc::time::range{0, 1}));
    XCTAssertEqual(track.modules().begin()->second.size(), 1);
    XCTAssertEqual(track.modules().begin()->second.at(0), module1);

    track.push_back_module(module2, {0, 1});

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

- (void)test_erase_modules_for_range {
    proc::track track;

    proc::module module1{[] { return proc::module::processors_t{}; }};
    proc::module module1b{[] { return proc::module::processors_t{}; }};
    proc::module module2{[] { return proc::module::processors_t{}; }};

    track.push_back_module(module1, {0, 1});
    track.push_back_module(module1b, {0, 1});
    track.push_back_module(module2, {1, 1});

    XCTAssertEqual(track.modules().size(), 2);

    track.erase_modules_for_range({0, 1});

    XCTAssertEqual(track.modules().size(), 1);
    XCTAssertEqual(track.modules().count({0, 1}), 0);
    XCTAssertEqual(track.modules().count({1, 1}), 1);
}

- (void)test_erase_module_with_range {
    proc::track track;

    proc::module module1{[] { return proc::module::processors_t{}; }};
    proc::module module1b{[] { return proc::module::processors_t{}; }};

    track.push_back_module(module1, {0, 1});
    track.push_back_module(module1b, {0, 1});

    XCTAssertEqual(track.modules().size(), 1);
    XCTAssertEqual(track.modules().at({0, 1}).size(), 2);

    track.erase_module(module1, {0, 1});

    XCTAssertEqual(track.modules().size(), 1);
    XCTAssertEqual(track.modules().at({0, 1}).size(), 1);
    XCTAssertEqual(track.modules().at({0, 1}).at(0), module1b);
}

- (void)test_erase_module_at {
    proc::track track;

    proc::module module1{[] { return proc::module::processors_t{}; }};
    proc::module module2{[] { return proc::module::processors_t{}; }};
    proc::module module3{[] { return proc::module::processors_t{}; }};

    track.push_back_module(module1, {0, 1});
    track.push_back_module(module2, {0, 1});
    track.push_back_module(module3, {0, 1});

    track.erase_module_at(1, {0, 1});

    XCTAssertEqual(track.modules().at({0, 1}).size(), 2);
    XCTAssertEqual(track.modules().at({0, 1}).at(0), module1);
    XCTAssertEqual(track.modules().at({0, 1}).at(1), module3);
}

- (void)test_total_range {
    proc::track track;

    XCTAssertFalse(track.total_range());

    track.push_back_module(proc::module{[] { return proc::module::processors_t{}; }}, {0, 1});

    XCTAssertEqual(track.total_range(), (proc::time::range{0, 1}));

    track.push_back_module(proc::module{[] { return proc::module::processors_t{}; }}, {1, 1});

    XCTAssertEqual(track.total_range(), (proc::time::range{0, 2}));

    track.push_back_module(proc::module{[] { return proc::module::processors_t{}; }}, {99, 1});

    XCTAssertEqual(track.total_range(), (proc::time::range{0, 100}));

    track.push_back_module(proc::module{[] { return proc::module::processors_t{}; }}, {-10, 1});

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
    track.push_back_module(std::move(module), {0, 1});

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
