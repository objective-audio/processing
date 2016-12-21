//
//  yas_processing_module_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing_test_utils.h"

using namespace yas;

@interface yas_processing_module_tests : XCTestCase

@end

@implementation yas_processing_module_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create_module {
    test::module module;
    
    XCTAssertTrue(module);
}

- (void)test_create_null {
    test::module module{nullptr};
    
    XCTAssertFalse(module);
}

- (void)test_offset {
    test::module module;
    
    module.set_frame_offset(5);
    
    XCTAssertEqual(module.frame_offset(), 5);
}

- (void)test_process_called {
    test::module module;
    processing::stream stream{{.start_frame = 23, .length = 456}};
    
    processing::time_range called_time_range;
    
    module.set_process_handler([&called_time_range](processing::stream &stream){
        called_time_range = stream.time_range();
    });
    
    module.process(stream);
    
    XCTAssertEqual(called_time_range.start_frame, 23);
    XCTAssertEqual(called_time_range.length, 456);
}

@end
