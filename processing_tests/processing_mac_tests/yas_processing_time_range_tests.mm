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
    auto range = time::range{.frame = 100, .length = 200};

    XCTAssertEqual(range.frame, 100);
    XCTAssertEqual(range.length, 200);
    XCTAssertEqual(range.next_frame(), 300);
}

- (void)test_create_empty_time_range {
    time::range range;
    
    XCTAssertEqual(range.frame, 0);
    XCTAssertEqual(range.length, 0);
}

- (void)test_equal_time_range {
    auto range1a = time::range{.frame = 12, .length = 345};
    auto range1b = time::range{.frame = 12, .length = 345};
    auto range2 = time::range{.frame = 12, .length = 678};
    auto range3 = time::range{.frame = 67, .length = 345};
    auto range4 = time::range{.frame = 98, .length = 765};

    XCTAssertTrue(range1a == range1b);
    XCTAssertFalse(range1a == range2);
    XCTAssertFalse(range1a == range3);
    XCTAssertFalse(range1a == range4);
}

- (void)test_not_equal_time_range {
    auto range1a = time::range{.frame = 12, .length = 345};
    auto range1b = time::range{.frame = 12, .length = 345};
    auto range2 = time::range{.frame = 12, .length = 678};
    auto range3 = time::range{.frame = 67, .length = 345};
    auto range4 = time::range{.frame = 98, .length = 765};

    XCTAssertFalse(range1a != range1b);
    XCTAssertTrue(range1a != range2);
    XCTAssertTrue(range1a != range3);
    XCTAssertTrue(range1a != range4);
}

- (void)test_less_than_time_range {
    auto range1a = time::range{.frame = 12, .length = 345};
    auto range1b = time::range{.frame = 12, .length = 345};
    auto range2 = time::range{.frame = 12, .length = 344};
    auto range3 = time::range{.frame = 12, .length = 346};
    auto range4 = time::range{.frame = 11, .length = 400};
    auto range5 = time::range{.frame = 13, .length = 300};

    XCTAssertFalse(range1a < range1b);
    XCTAssertFalse(range1a < range2);
    XCTAssertTrue(range1a < range3);
    XCTAssertFalse(range1a < range4);
    XCTAssertTrue(range1a < range5);
}

- (void)test_can_combine {
    auto range1 = time::range{.frame = 0, .length = 2};
    auto range2 = time::range{.frame = 2, .length = 2};
    auto range3 = time::range{.frame = 3, .length = 2};
    auto range4 = time::range{.frame = 1, .length = 2};
    auto range5 = time::range{.frame = -1, .length = 4};
    auto range6 = time::range{.frame = 1, .length = 0};
    
    XCTAssertTrue(range1.can_combine({.frame = 0, .length = 2}));
    
    XCTAssertTrue(range1.can_combine(range2));
    XCTAssertFalse(range1.can_combine(range3));
    XCTAssertTrue(range1.can_combine(range4));
    XCTAssertTrue(range1.can_combine(range5));
    XCTAssertFalse(range1.can_combine(range6));
    
    XCTAssertTrue(range2.can_combine(range1));
    XCTAssertFalse(range3.can_combine(range1));
    XCTAssertTrue(range4.can_combine(range1));
    XCTAssertTrue(range5.can_combine(range1));
    XCTAssertFalse(range6.can_combine(range1));
}

- (void)test_combine {
    auto range1 = time::range{.frame = 0, .length = 2};
    auto range2 = time::range{.frame = 2, .length = 2};
    auto range3 = time::range{.frame = 3, .length = 2};
    auto range4 = time::range{.frame = 1, .length = 2};
    auto range5 = time::range{.frame = -1, .length = 4};
    
    XCTAssertTrue((range1.combine(range2) == time::range{.frame = 0, .length = 4}));
    XCTAssertFalse(range1.combine(range3));
    XCTAssertTrue((range1.combine(range4) == time::range{.frame = 0, .length = 3}));
    XCTAssertTrue((range1.combine(range5) == time::range{.frame = -1, .length = 4}));
    
    XCTAssertTrue((range2.combine(range1) == time::range{.frame = 0, .length = 4}));
    XCTAssertFalse(range3.combine(range1));
    XCTAssertTrue((range4.combine(range1) == time::range{.frame = 0, .length = 3}));
    XCTAssertTrue((range5.combine(range1) == time::range{.frame = -1, .length = 4}));
}

- (void)test_is_contain {
    auto range1 = time::range{.frame = 5, .length = 2};
    
    XCTAssertTrue(range1.is_contain({.frame = 5, .length = 2}));
    XCTAssertFalse(range1.is_contain({.frame = 4, .length = 4}));
    XCTAssertFalse(range1.is_contain({.frame = 4, .length = 3}));
    XCTAssertFalse(range1.is_contain({.frame = 6, .length = 3}));
    XCTAssertFalse(range1.is_contain({.frame = 3, .length = 1}));
    XCTAssertFalse(range1.is_contain({.frame = 4, .length = 1}));
    XCTAssertTrue(range1.is_contain({.frame = 5, .length = 1}));
    XCTAssertFalse(range1.is_contain({.frame = 7, .length = 1}));
    XCTAssertTrue(range1.is_contain({.frame = 5, .length = 0}));
    XCTAssertTrue(range1.is_contain({.frame = 6, .length = 0}));
    XCTAssertFalse(range1.is_contain({.frame = 7, .length = 0}));
    
    auto range2 = time::range{.frame = 10, .length = 0};
    
    XCTAssertFalse(range2.is_contain({.frame = 10, .length = 0}));
    XCTAssertFalse(range2.is_contain({.frame = 10, .length = 1}));
    XCTAssertFalse(range2.is_contain({.frame = 9, .length = 2}));
}

- (void)test_is_overlap {
    auto range1 = time::range{.frame = 7, .length = 2};
    
    XCTAssertTrue(range1.is_overlap({.frame = 7, .length = 2}));
    XCTAssertTrue(range1.is_overlap({.frame = 6, .length = 2}));
    XCTAssertTrue(range1.is_overlap({.frame = 8, .length = 2}));
    XCTAssertTrue(range1.is_overlap({.frame = 6, .length = 3}));
    XCTAssertTrue(range1.is_overlap({.frame = 7, .length = 3}));
    XCTAssertTrue(range1.is_overlap({.frame = 7, .length = 1}));
    XCTAssertTrue(range1.is_overlap({.frame = 8, .length = 1}));
    XCTAssertFalse(range1.is_overlap({.frame = 8, .length = 0}));
    XCTAssertFalse(range1.is_overlap({.frame = 6, .length = 1}));
    XCTAssertFalse(range1.is_overlap({.frame = 9, .length = 1}));
    
    auto range2 = time::range{.frame = 7, .length = 0};
    
    XCTAssertFalse(range2.is_overlap({.frame = 7, .length = 0}));
    XCTAssertFalse(range2.is_overlap({.frame = 6, .length = 2}));
}

@end
