//
//  yas_processing_number_event_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_number_event_tests : XCTestCase

@end

@implementation yas_processing_number_event_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create_with_value {
    processing::number_event event{1};

    XCTAssertTrue(event);
}

- (void)test_create_null {
    processing::number_event event = nullptr;

    XCTAssertFalse(event);
}

- (void)test_get {
    int16_t const value = 3;
    processing::number_event event{value};

    XCTAssertEqual(event.get<int16_t>(), 3);
}

- (void)test_sample_byte_count {
    XCTAssertEqual(processing::number_event{int8_t(0)}.sample_byte_count(), 1);
    XCTAssertEqual(processing::number_event{double(0.0)}.sample_byte_count(), 8);
    XCTAssertEqual(processing::number_event{processing::boolean{false}}.sample_byte_count(), 1);
}

- (void)test_sample_type {
    XCTAssertTrue(processing::number_event{int8_t(0)}.sample_type() == typeid(int8_t));
    XCTAssertTrue(processing::number_event{double(0.0)}.sample_type() == typeid(double));
    XCTAssertTrue(processing::number_event{processing::boolean{false}}.sample_type() == typeid(processing::boolean));
}

@end
