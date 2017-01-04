//
//  yas_processing_time_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;

@interface yas_processing_time_tests : XCTestCase

@end

@implementation yas_processing_time_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create_range_time {
    processing::time time{1, 2};

    XCTAssertTrue(time);
    XCTAssertTrue(time.type() == typeid(processing::time::range));
}

- (void)test_create_frame_time {
    processing::time time{3};

    XCTAssertTrue(time);
    XCTAssertTrue(time.type() == typeid(processing::time::frame));

    auto const &frame = time.get<processing::time::frame>();

    XCTAssertEqual(frame, 3);
}

- (void)test_create_any_time {
    processing::time time;

    XCTAssertTrue(time);
    XCTAssertTrue(time.type() == typeid(processing::time::any));
}

- (void)test_create_null {
    processing::time time = nullptr;

    XCTAssertFalse(time);
}

- (void)test_make_range_time {
    auto time = processing::make_range_time(1, 2);

    XCTAssertTrue(time);
    XCTAssertTrue(time.type() == typeid(processing::time::range));
}

- (void)test_make_time_frame {
    auto time = processing::make_frame_time(3);

    XCTAssertTrue(time);
    XCTAssertTrue(time.type() == typeid(processing::time::frame));
}

- (void)test_make_time_any {
    auto time = processing::make_any_time();

    XCTAssertTrue(time);
    XCTAssertTrue(time.type() == typeid(processing::time::any));
}

- (void)test_get_range_time {
    processing::time time{1, 2};

    auto const &range = time.get<processing::time::range>();

    XCTAssertEqual(range.frame, 1);
    XCTAssertEqual(range.length, 2);
}

- (void)test_get_frame_time {
    processing::time time{3};

    auto const &frame = time.get<processing::time::frame>();

    XCTAssertEqual(frame, 3);
}

- (void)test_is_equal_range_time {
    auto time1a = processing::make_range_time(0, 1);
    auto time1b = processing::make_range_time(0, 1);
    auto time2 = processing::make_range_time(0, 2);
    auto time3 = processing::make_range_time(1, 1);

    XCTAssertTrue(time1a == time1a);
    XCTAssertTrue(time1a == time1b);
    XCTAssertFalse(time1a == time2);
    XCTAssertFalse(time1a == time3);

    XCTAssertFalse(time1a != time1a);
    XCTAssertFalse(time1a != time1b);
    XCTAssertTrue(time1a != time2);
    XCTAssertTrue(time1a != time3);
}

- (void)test_is_equal_frame_time {
    auto time1a = processing::make_frame_time(1);
    auto time1b = processing::make_frame_time(1);
    auto time2 = processing::make_frame_time(2);
    
    XCTAssertTrue(time1a == time1a);
    XCTAssertTrue(time1a == time1b);
    XCTAssertFalse(time1a == time2);
    
    XCTAssertFalse(time1a != time1a);
    XCTAssertFalse(time1a != time1b);
    XCTAssertTrue(time1a != time2);
}

- (void)test_is_equal_any_time {
    auto time1a = processing::make_any_time();
    auto time1b = processing::make_any_time();
    
    XCTAssertTrue(time1a == time1a);
    XCTAssertTrue(time1a == time1b);
    
    XCTAssertFalse(time1a != time1a);
    XCTAssertFalse(time1a != time1b);
}

@end
