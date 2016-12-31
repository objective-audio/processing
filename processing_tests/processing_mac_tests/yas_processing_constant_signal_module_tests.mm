//
//  yas_processing_constant_signal_module_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;

@interface yas_processing_constant_signal_module_tests : XCTestCase

@end

@implementation yas_processing_constant_signal_module_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_make_module {
    double const value = 1.0;
    auto module = processing::constant::make_signal_module(value);
    
    XCTAssertTrue(module);
}

@end
