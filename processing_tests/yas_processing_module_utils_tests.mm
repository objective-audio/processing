//
//  yas_processing_module_utils_tests.mm
//

#import <XCTest/XCTest.h>
#import <processing/yas_processing_module_utils.h>

using namespace yas;
using namespace yas::proc;

@interface yas_processing_module_utils_tests : XCTestCase

@end

@implementation yas_processing_module_utils_tests

- (void)test_module_frame {
    XCTAssertEqual(module_frame(0, 0), 0);
    XCTAssertEqual(module_frame(1, 0), 1);
    XCTAssertEqual(module_frame(0, 1), -1);
    XCTAssertEqual(module_frame(1, 1), 0);
}

@end
