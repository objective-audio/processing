//
//  yas_processing_math_modules_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;
using namespace yas::processing;

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
    auto plus_module = make_signal_module<int16_t>(math::kind::plus);
    XCTAssertTrue(plus_module);

    auto minus_module = make_signal_module<int16_t>(math::kind::minus);
    XCTAssertTrue(minus_module);

    auto multiply_module = make_signal_module<int16_t>(math::kind::multiply);
    XCTAssertTrue(multiply_module);

    auto divide_module = make_signal_module<int16_t>(math::kind::divide);
    XCTAssertTrue(divide_module);
}

- (void)test_plus_process {
    auto left_in_module = processing::constant::make_signal_module<int16_t>(1);
    auto right_in_module = processing::constant::make_signal_module<int16_t>(10);
    auto calc_module = make_signal_module<int16_t>(math::kind::plus);
    processing::stream stream{sync_source{1, 5}};

    left_in_module.connect_output(to_connector_index(constant::output_key::out), 0);
    right_in_module.connect_output(to_connector_index(constant::output_key::out), 1);
    calc_module.connect_input(to_connector_index(math::input_key::left), 0);
    calc_module.connect_input(to_connector_index(math::input_key::right), 1);
    calc_module.connect_output(to_connector_index(constant::output_key::out), 2);

    left_in_module.process({1, 2}, stream);
    right_in_module.process({2, 2}, stream);
    calc_module.process({0, 5}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &events = stream.channel(2).events();

    XCTAssertEqual(events.size(), 1);

    auto const signal = cast<processing::signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<int16_t>();

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
    auto calc_module = make_signal_module<int16_t>(math::kind::minus);
    processing::stream stream{sync_source{1, 5}};

    left_in_module.connect_output(to_connector_index(constant::output_key::out), 0);
    right_in_module.connect_output(to_connector_index(constant::output_key::out), 1);
    calc_module.connect_input(to_connector_index(math::input_key::left), 0);
    calc_module.connect_input(to_connector_index(math::input_key::right), 1);
    calc_module.connect_output(to_connector_index(constant::output_key::out), 2);

    left_in_module.process({1, 2}, stream);
    right_in_module.process({2, 2}, stream);
    calc_module.process({0, 5}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &events = stream.channel(2).events();

    XCTAssertEqual(events.size(), 1);

    auto const &signal = cast<processing::signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<int16_t>();

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
    auto calc_module = make_signal_module<int16_t>(math::kind::multiply);
    processing::stream stream{sync_source{1, 5}};

    left_in_module.connect_output(to_connector_index(constant::output_key::out), 0);
    right_in_module.connect_output(to_connector_index(constant::output_key::out), 1);
    calc_module.connect_input(to_connector_index(math::input_key::left), 0);
    calc_module.connect_input(to_connector_index(math::input_key::right), 1);
    calc_module.connect_output(to_connector_index(constant::output_key::out), 2);

    left_in_module.process({1, 2}, stream);
    right_in_module.process({2, 2}, stream);
    calc_module.process({0, 5}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &events = stream.channel(2).events();

    XCTAssertEqual(events.size(), 1);

    auto const &signal = cast<processing::signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<int16_t>();

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
    auto calc_module = make_signal_module<int16_t>(math::kind::divide);
    processing::stream stream{sync_source{1, 5}};

    left_in_module.connect_output(to_connector_index(constant::output_key::out), 0);
    right_in_module.connect_output(to_connector_index(constant::output_key::out), 1);
    calc_module.connect_input(to_connector_index(math::input_key::left), 0);
    calc_module.connect_input(to_connector_index(math::input_key::right), 1);
    calc_module.connect_output(to_connector_index(constant::output_key::out), 2);

    left_in_module.process({1, 2}, stream);
    right_in_module.process({2, 2}, stream);
    calc_module.process({0, 5}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &events = stream.channel(2).events();

    XCTAssertEqual(events.size(), 1);

    auto const &signal = cast<processing::signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<int16_t>();

    XCTAssertEqual(vec.size(), 5);
    XCTAssertEqual(vec[0], 0);
    XCTAssertEqual(vec[1], 0);
    XCTAssertEqual(vec[2], 2);
    XCTAssertEqual(vec[3], 0);
    XCTAssertEqual(vec[4], 0);
}

@end
