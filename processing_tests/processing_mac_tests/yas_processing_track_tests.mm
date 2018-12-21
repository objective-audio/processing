//
//  yas_processing_track_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;

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

    proc::module module1{proc::module::processors_t{}};
    proc::module module2{proc::module::processors_t{}};

    track.insert_module({0, 1}, std::move(module1));
    track.insert_module({1, 1}, std::move(module2));

    XCTAssertEqual(track.modules().size(), 2);

    if (auto const &const_track = track) {
        std::size_t idx = 0;
        for (auto const &pair : const_track.modules()) {
            auto const &time_range = pair.first;
            auto const &module = pair.second;

            XCTAssertTrue(module);

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

    proc::module module1{proc::module::processors_t{}};
    proc::module module2{proc::module::processors_t{}};

    track.insert_module({0, 1}, module1);
    track.insert_module({1, 1}, module2);

    XCTAssertEqual(track.modules().size(), 2);

    track.remove_module(module1);

    XCTAssertEqual(track.modules().size(), 1);
    XCTAssertEqual(track.modules().begin()->second, module2);
}

- (void)test_total_range {
    proc::track track;

    XCTAssertFalse(track.total_range());

    track.insert_module({0, 1}, proc::module{proc::module::processors_t{}});

    XCTAssertEqual(track.total_range(), (proc::time::range{0, 1}));

    track.insert_module({1, 1}, proc::module{proc::module::processors_t{}});

    XCTAssertEqual(track.total_range(), (proc::time::range{0, 2}));

    track.insert_module({99, 1}, proc::module{proc::module::processors_t{}});

    XCTAssertEqual(track.total_range(), (proc::time::range{0, 100}));

    track.insert_module({-10, 1}, proc::module{proc::module::processors_t{}});

    XCTAssertEqual(track.total_range(), (proc::time::range{-10, 110}));
}

@end
