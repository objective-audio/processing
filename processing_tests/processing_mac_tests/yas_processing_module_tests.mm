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
    test::module module{processing::time_range{.start_frame = 1, .length = 2}};
    
    XCTAssertTrue(module);
    XCTAssertTrue((module.time_range() == processing::time_range{.start_frame = 1, .length = 2}));
    XCTAssertEqual(module.input_connectors().size(), 0);
    XCTAssertEqual(module.output_connectors().size(), 0);
}

- (void)test_create_null {
    test::module module{nullptr};
    
    XCTAssertFalse(module);
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

- (void)test_connect_input {
    test::module module;
    
    module.connect_input("a", 1);
    
    XCTAssertEqual(module.input_connectors().size(), 1);
    XCTAssertEqual(module.input_connectors().count("a"), 1);
    XCTAssertEqual(module.input_connectors().at("a").channel_index, 1);
}

- (void)test_connect_output {
    test::module module;
    
    module.connect_output("b", 2);
    
    XCTAssertEqual(module.output_connectors().size(), 1);
    XCTAssertEqual(module.output_connectors().count("b"), 1);
    XCTAssertEqual(module.output_connectors().at("b").channel_index, 2);
}

- (void)test_disconnect_input {
    test::module module;
    
    module.connect_input("c", 3);
    
    module.disconnect_input("c1");
    
    XCTAssertEqual(module.input_connectors().size(), 1);
    
    module.disconnect_input("c");
    
    XCTAssertEqual(module.input_connectors().size(), 0);
}

- (void)test_disconnect_output {
    test::module module;
    
    module.connect_output("d", 3);
    
    module.disconnect_output("d1");
    
    XCTAssertEqual(module.output_connectors().size(), 1);
    
    module.disconnect_output("d");
    
    XCTAssertEqual(module.output_connectors().size(), 0);
}

@end
