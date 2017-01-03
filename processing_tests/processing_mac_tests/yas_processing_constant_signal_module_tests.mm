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

- (void)test_process {
    int64_t const value = 5;
    
    auto module = processing::constant::make_signal_module(value);
    module.connect_output(processing::constant::out_connector_key, 0);
    
    processing::stream stream{{.start_frame = 0, .length = 2}};
    
    module.process({.start_frame = 0, .length = 2}, stream);
    
    XCTAssertTrue(stream.has_channel(0));
    
    auto const &channel = stream.channel(0);
    
    XCTAssertEqual(channel.buffers().size(), 1);
    
    auto const &buffer = channel.buffers().begin()->second;
    
    XCTAssertEqual(buffer.size(), 2);
    
    auto const &vec = processing::get_vector<int64_t>(buffer);
    
    XCTAssertEqual(vec[0], 5);
    XCTAssertEqual(vec[1], 5);
}

@end
