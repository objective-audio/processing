//
//  yas_processing_cast_module_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_cast_module_tests : XCTestCase

@end

@implementation yas_processing_cast_module_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create {
    auto module = cast::make_signal_module<int8_t, float>();

    XCTAssertTrue(module);
}

- (void)test_process {
    auto constant_module1 = constant::make_signal_module(int8_t(1));
    constant_module1.connect_output(constant::out_connector_key, 0);

    auto constant_module2 = constant::make_signal_module(int8_t(2));
    constant_module2.connect_output(constant::out_connector_key, 0);

    auto cast_module = cast::make_signal_module<int8_t, float>();
    cast_module.connect_input(cast::in_connector_key, 0);
    cast_module.connect_output(cast::out_connector_key, 1);

    stream stream;

    constant_module1.process({0, 1}, stream);
    constant_module2.process({1, 1}, stream);
    cast_module.process({0, 2}, stream);

    XCTAssertTrue(stream.has_channel(1));

    auto const &channel = stream.channel(1);
    XCTAssertEqual(channel.events().size(), 1);

    auto &event = (*channel.events().begin()).second;
    auto const signal = cast<signal_event>(event);
    XCTAssertTrue(signal);
    XCTAssertTrue(signal.sample_type() == typeid(float));
    XCTAssertEqual(signal.size(), 2);
    auto const *data = signal.data<float>();
    XCTAssertEqual(data[0], 1.0);
    XCTAssertEqual(data[1], 2.0);
}

@end
