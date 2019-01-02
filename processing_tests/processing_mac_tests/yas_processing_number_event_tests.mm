//
//  yas_processing_number_event_tests.mm
//

#import <XCTest/XCTest.h>
#import <cpp_utils/yas_boolean.h>
#import "yas_processing.h"

using namespace yas;
using namespace yas::proc;

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
    proc::number_event event{1};

    XCTAssertTrue(event);
}

- (void)test_create_null {
    proc::number_event event = nullptr;

    XCTAssertFalse(event);
}

- (void)test_get {
    int16_t const value = 3;
    proc::number_event event{value};

    XCTAssertEqual(event.get<int16_t>(), 3);
}

- (void)test_sample_byte_count {
    XCTAssertEqual(number_event{int8_t(0)}.sample_byte_count(), 1);
    XCTAssertEqual(number_event{double(0.0)}.sample_byte_count(), 8);
    XCTAssertEqual(number_event{boolean{false}}.sample_byte_count(), 1);
}

- (void)test_sample_type {
    XCTAssertTrue(number_event{int8_t(0)}.sample_type() == typeid(int8_t));
    XCTAssertTrue(number_event{double(0.0)}.sample_type() == typeid(double));
    XCTAssertTrue(number_event{boolean{false}}.sample_type() == typeid(boolean));
}

- (void)test_copy {
    auto src_event = number_event{uint32_t(11)};
    auto copied_event = cast<number_event>(src_event.copy());

    XCTAssertEqual(src_event, copied_event);
    XCTAssertEqual(copied_event.get<uint32_t>(), 11);
}

@end
