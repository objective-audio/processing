//
//  yas_processing_math_modules_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;

@interface yas_processing_math_modules_tests : XCTestCase

@end

@implementation yas_processing_math_modules_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create_signal_modules {
    auto plus_module = processing::math::make_plus_signal_module<int16_t>();
    XCTAssertTrue(plus_module);

    auto minus_module = processing::math::make_minus_signal_module<int16_t>();
    XCTAssertTrue(minus_module);

    auto multiply_module = processing::math::make_multiply_signal_module<int16_t>();
    XCTAssertTrue(multiply_module);

    auto divide_module = processing::math::make_divide_signal_module<int16_t>();
    XCTAssertTrue(divide_module);
}

- (void)test_plus_process {
    auto left_in_module = processing::constant::make_signal_module<int16_t>(1);
    auto right_in_module = processing::constant::make_signal_module<int16_t>(10);
    auto plus_module = processing::math::make_plus_signal_module<int16_t>();
    auto stream = processing::stream{{.start_frame = 0, .length = 5}};

    left_in_module.connect_output(processing::constant::out_connector_key, 0);
    right_in_module.connect_output(processing::constant::out_connector_key, 1);
    plus_module.connect_input(processing::math::left_in_connector_key, 0);
    plus_module.connect_input(processing::math::right_in_connector_key, 1);
    plus_module.connect_output(processing::constant::out_connector_key, 2);

    left_in_module.process({.start_frame = 1, .length = 2}, stream);
    right_in_module.process({.start_frame = 2, .length = 2}, stream);
    plus_module.process({.start_frame = 0, .length = 5}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &buffers = stream.channel(2).buffers();

    XCTAssertEqual(buffers.size(), 1);

    auto const &buffer = (*buffers.begin()).second;
    auto const &vec = processing::get_vector<int16_t>(buffer);

    XCTAssertEqual(vec.size(), 5);
    XCTAssertEqual(vec[0], 0);
    XCTAssertEqual(vec[1], 1);
    XCTAssertEqual(vec[2], 11);
    XCTAssertEqual(vec[3], 10);
    XCTAssertEqual(vec[4], 0);
}

- (void)test_minus_process {
    auto left_in_module = processing::constant::make_signal_module<int16_t>(100);
    auto right_in_module = processing::constant::make_signal_module<int16_t>(10);
    auto plus_module = processing::math::make_minus_signal_module<int16_t>();
    auto stream = processing::stream{{.start_frame = 0, .length = 5}};
    
    left_in_module.connect_output(processing::constant::out_connector_key, 0);
    right_in_module.connect_output(processing::constant::out_connector_key, 1);
    plus_module.connect_input(processing::math::left_in_connector_key, 0);
    plus_module.connect_input(processing::math::right_in_connector_key, 1);
    plus_module.connect_output(processing::constant::out_connector_key, 2);
    
    left_in_module.process({.start_frame = 1, .length = 2}, stream);
    right_in_module.process({.start_frame = 2, .length = 2}, stream);
    plus_module.process({.start_frame = 0, .length = 5}, stream);
    
    XCTAssertTrue(stream.has_channel(2));
    
    auto const &buffers = stream.channel(2).buffers();
    
    XCTAssertEqual(buffers.size(), 1);
    
    auto const &buffer = (*buffers.begin()).second;
    auto const &vec = processing::get_vector<int16_t>(buffer);
    
    XCTAssertEqual(vec.size(), 5);
    XCTAssertEqual(vec[0], 0);
    XCTAssertEqual(vec[1], 100);
    XCTAssertEqual(vec[2], 90);
    XCTAssertEqual(vec[3], -10);
    XCTAssertEqual(vec[4], 0);
}

- (void)test_multiply_process {
    auto left_in_module = processing::constant::make_signal_module<int16_t>(2);
    auto right_in_module = processing::constant::make_signal_module<int16_t>(4);
    auto plus_module = processing::math::make_multiply_signal_module<int16_t>();
    auto stream = processing::stream{{.start_frame = 0, .length = 5}};
    
    left_in_module.connect_output(processing::constant::out_connector_key, 0);
    right_in_module.connect_output(processing::constant::out_connector_key, 1);
    plus_module.connect_input(processing::math::left_in_connector_key, 0);
    plus_module.connect_input(processing::math::right_in_connector_key, 1);
    plus_module.connect_output(processing::constant::out_connector_key, 2);
    
    left_in_module.process({.start_frame = 1, .length = 2}, stream);
    right_in_module.process({.start_frame = 2, .length = 2}, stream);
    plus_module.process({.start_frame = 0, .length = 5}, stream);
    
    XCTAssertTrue(stream.has_channel(2));
    
    auto const &buffers = stream.channel(2).buffers();
    
    XCTAssertEqual(buffers.size(), 1);
    
    auto const &buffer = (*buffers.begin()).second;
    auto const &vec = processing::get_vector<int16_t>(buffer);
    
    XCTAssertEqual(vec.size(), 5);
    XCTAssertEqual(vec[0], 0);
    XCTAssertEqual(vec[1], 0);
    XCTAssertEqual(vec[2], 8);
    XCTAssertEqual(vec[3], 0);
    XCTAssertEqual(vec[4], 0);
}

- (void)test_divide_process {
    auto left_in_module = processing::constant::make_signal_module<int16_t>(4);
    auto right_in_module = processing::constant::make_signal_module<int16_t>(2);
    auto plus_module = processing::math::make_divide_signal_module<int16_t>();
    auto stream = processing::stream{{.start_frame = 0, .length = 5}};
    
    left_in_module.connect_output(processing::constant::out_connector_key, 0);
    right_in_module.connect_output(processing::constant::out_connector_key, 1);
    plus_module.connect_input(processing::math::left_in_connector_key, 0);
    plus_module.connect_input(processing::math::right_in_connector_key, 1);
    plus_module.connect_output(processing::constant::out_connector_key, 2);
    
    left_in_module.process({.start_frame = 1, .length = 2}, stream);
    right_in_module.process({.start_frame = 2, .length = 2}, stream);
    plus_module.process({.start_frame = 0, .length = 5}, stream);
    
    XCTAssertTrue(stream.has_channel(2));
    
    auto const &buffers = stream.channel(2).buffers();
    
    XCTAssertEqual(buffers.size(), 1);
    
    auto const &buffer = (*buffers.begin()).second;
    auto const &vec = processing::get_vector<int16_t>(buffer);
    
    XCTAssertEqual(vec.size(), 5);
    XCTAssertEqual(vec[0], 0);
    XCTAssertEqual(vec[1], 0);
    XCTAssertEqual(vec[2], 2);
    XCTAssertEqual(vec[3], 0);
    XCTAssertEqual(vec[4], 0);
}

@end
