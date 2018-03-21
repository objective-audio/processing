//
//  yas_processing_timeline_module_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;
using namespace yas::proc;

@interface yas_processing_timeline_module_tests : XCTestCase

@end

@implementation yas_processing_timeline_module_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_make_module {
    timeline timeline;
    
    auto module = make_module(timeline);
    
    XCTAssertTrue(module);
}

- (void)test_process {
    timeline parent_timeline;
    timeline child_timeline;
    
#warning todo
}

@end
