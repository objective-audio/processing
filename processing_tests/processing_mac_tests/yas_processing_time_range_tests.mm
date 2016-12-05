//
//  yas_processing_time_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing_time_range.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_time_tests : XCTestCase

@end

@implementation yas_processing_time_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create_time_range {
    auto range = time_range{.start_frame = 100, .length = 200};

    XCTAssertEqual(range.start_frame, 100);
    XCTAssertEqual(range.length, 200);
    XCTAssertEqual(range.next_frame(), 300);
}

- (void)test_equal_time_range {
    auto range1a = time_range{.start_frame = 12, .length = 345};
    auto range1b = time_range{.start_frame = 12, .length = 345};
    auto range2 = time_range{.start_frame = 12, .length = 678};
    auto range3 = time_range{.start_frame = 67, .length = 345};
    auto range4 = time_range{.start_frame = 98, .length = 765};

    XCTAssertTrue(range1a == range1b);
    XCTAssertFalse(range1a == range2);
    XCTAssertFalse(range1a == range3);
    XCTAssertFalse(range1a == range4);
}

- (void)test_not_equal_time_range {
    auto range1a = time_range{.start_frame = 12, .length = 345};
    auto range1b = time_range{.start_frame = 12, .length = 345};
    auto range2 = time_range{.start_frame = 12, .length = 678};
    auto range3 = time_range{.start_frame = 67, .length = 345};
    auto range4 = time_range{.start_frame = 98, .length = 765};

    XCTAssertFalse(range1a != range1b);
    XCTAssertTrue(range1a != range2);
    XCTAssertTrue(range1a != range3);
    XCTAssertTrue(range1a != range4);
}

- (void)test_less_than_time_range {
    auto range1a = time_range{.start_frame = 12, .length = 345};
    auto range1b = time_range{.start_frame = 12, .length = 345};
    auto range2 = time_range{.start_frame = 12, .length = 344};
    auto range3 = time_range{.start_frame = 12, .length = 346};
    auto range4 = time_range{.start_frame = 11, .length = 400};
    auto range5 = time_range{.start_frame = 13, .length = 300};

    XCTAssertFalse(range1a < range1b);
    XCTAssertFalse(range1a < range2);
    XCTAssertTrue(range1a < range3);
    XCTAssertFalse(range1a < range4);
    XCTAssertTrue(range1a < range5);
}

@end
