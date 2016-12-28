//
//  yas_processing_track_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing_test_utils.h"

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
    processing::track track;

    XCTAssertTrue(track);
    XCTAssertEqual(track.modules().size(), 0);
}

- (void)test_create_null {
    processing::track track = nullptr;

    XCTAssertFalse(track);
}

- (void)test_insert_module {
    processing::track track;

    test::module module1{{.start_frame = 0}};
    module1.set_value(1);

    test::module module2{{.start_frame = 1}};
    module2.set_value(2);

    track.insert_module(std::move(module1));
    track.insert_module(std::move(module2));

    XCTAssertEqual(track.modules().size(), 2);

    if (auto const &const_track = track) {
        std::size_t idx = 0;
        for (auto const &pair : const_track.modules()) {
            auto const &time_range = pair.first;
            auto const &module = cast<test::module>(pair.second);

            XCTAssertTrue(module);

            if (idx == 0) {
                XCTAssertTrue((time_range == processing::time_range{.start_frame = 0}));
                XCTAssertEqual(module.value(), 1);
            } else if (idx == 1) {
                XCTAssertTrue((time_range == processing::time_range{.start_frame = 1}));
                XCTAssertEqual(module.value(), 2);
            } else {
                XCTFail();
            }

            ++idx;
        }
    }
}

@end
