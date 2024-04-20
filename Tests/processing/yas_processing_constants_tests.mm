//
//  yas_processing_constants_tests.mm
//

#import <XCTest/XCTest.h>
#import <processing/common/yas_processing_constants.h>

using namespace yas;
using namespace yas::proc;

@interface yas_processing_constants_tests : XCTestCase

@end

@implementation yas_processing_constants_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_zero_length {
    XCTAssertEqual(constant::zero_length, 0);
    XCTAssertTrue(typeid(constant::zero_length) == typeid(length_t));
}

@end
