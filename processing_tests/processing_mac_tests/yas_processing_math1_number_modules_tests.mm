//
//  yas_processing_math1_number_modules_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing_test_utils.h"

#import <iostream>

using namespace yas;
using namespace yas::processing;

namespace yas {
namespace test {
    static double constexpr two_pi = 2.0 * M_PI;

    static std::size_t constexpr process_length = 8;

    static double constexpr radian_input_data[process_length] = {
        0.0, 0.25 * two_pi, 0.5 * two_pi, 0.75 * two_pi, 1.0 * two_pi, 1.25 * two_pi, 1.5 * two_pi, 1.75 * two_pi,
    };

    static double constexpr linear_input_data[process_length]{
        -1.5, -1.0, -0.5, 0.0, 0.5, 1.0, 1.5, 2.0,
    };
}
}

@interface yas_processing_math1_number_modules_tests : XCTestCase

@end

@implementation yas_processing_math1_number_modules_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_make_number_module {
    XCTAssertTrue(make_number_module<double>(math1::kind::sin));
    XCTAssertTrue(make_number_module<double>(math1::kind::cos));
    XCTAssertTrue(make_number_module<double>(math1::kind::tan));
    XCTAssertTrue(make_number_module<double>(math1::kind::asin));
    XCTAssertTrue(make_number_module<double>(math1::kind::acos));
    XCTAssertTrue(make_number_module<double>(math1::kind::atan));

    XCTAssertTrue(make_number_module<double>(math1::kind::sinh));
    XCTAssertTrue(make_number_module<double>(math1::kind::cosh));
    XCTAssertTrue(make_number_module<double>(math1::kind::tanh));
    XCTAssertTrue(make_number_module<double>(math1::kind::asinh));
    XCTAssertTrue(make_number_module<double>(math1::kind::acosh));
    XCTAssertTrue(make_number_module<double>(math1::kind::atanh));

    XCTAssertTrue(make_number_module<double>(math1::kind::exp2));
    XCTAssertTrue(make_number_module<double>(math1::kind::expm1));
    XCTAssertTrue(make_number_module<double>(math1::kind::log));
    XCTAssertTrue(make_number_module<double>(math1::kind::log10));
    XCTAssertTrue(make_number_module<double>(math1::kind::log1p));
    XCTAssertTrue(make_number_module<double>(math1::kind::log2));

    XCTAssertTrue(make_number_module<double>(math1::kind::sqrt));
    XCTAssertTrue(make_number_module<double>(math1::kind::cbrt));
    XCTAssertTrue(make_number_module<double>(math1::kind::abs));

    XCTAssertTrue(make_number_module<double>(math1::kind::ceil));
    XCTAssertTrue(make_number_module<double>(math1::kind::floor));
    XCTAssertTrue(make_number_module<double>(math1::kind::trunc));
    XCTAssertTrue(make_number_module<double>(math1::kind::round));
}

- (void)test_sin {
    channel_index_t const ch_idx = 3;

    auto module = test::make_number_module<double>(math1::kind::sin, ch_idx);
    auto stream =
        test::make_number_stream<double>(test::radian_input_data, time::range{0, test::process_length}, ch_idx);

    module.process(time::range{0, test::process_length}, stream);

    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();

    XCTAssertEqual(events.size(), 8);

    auto event_iterator = events.begin();

    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sin(test::radian_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sin(test::radian_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sin(test::radian_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sin(test::radian_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sin(test::radian_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sin(test::radian_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sin(test::radian_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sin(test::radian_input_data[7]), 0.01);
}

- (void)test_cos {
    channel_index_t const ch_idx = 4;

    auto module = test::make_number_module<double>(math1::kind::cos, ch_idx);
    auto stream =
        test::make_number_stream<double>(test::radian_input_data, time::range{0, test::process_length}, ch_idx);

    module.process(time::range{0, test::process_length}, stream);

    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();

    XCTAssertEqual(events.size(), 8);

    auto event_iterator = events.begin();

    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cos(test::radian_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cos(test::radian_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cos(test::radian_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cos(test::radian_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cos(test::radian_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cos(test::radian_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cos(test::radian_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cos(test::radian_input_data[7]), 0.01);
}

- (void)test_tan {
    channel_index_t const ch_idx = 5;

    auto module = test::make_number_module<double>(math1::kind::tan, ch_idx);
    auto stream =
        test::make_number_stream<double>(test::radian_input_data, time::range{0, test::process_length}, ch_idx);

    module.process(time::range{0, test::process_length}, stream);

    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();

    XCTAssertEqual(events.size(), 8);

    auto event_iterator = events.begin();

    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::tan(test::radian_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::tan(test::radian_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::tan(test::radian_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::tan(test::radian_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::tan(test::radian_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::tan(test::radian_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::tan(test::radian_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::tan(test::radian_input_data[7]), 0.01);
}

- (void)test_asin {
    channel_index_t const ch_idx = 6;

    auto module = test::make_number_module<double>(math1::kind::asin, ch_idx);
    auto stream =
        test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);

    module.process(time::range{0, test::process_length}, stream);

    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();

    XCTAssertEqual(events.size(), 8);

    auto event_iterator = events.begin();

    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::asin(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::asin(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::asin(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::asin(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::asin(test::linear_input_data[5]), 0.01);
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
}

- (void)test_acos {
    channel_index_t const ch_idx = 7;

    auto module = test::make_number_module<double>(math1::kind::acos, ch_idx);
    auto stream =
        test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);

    module.process(time::range{0, test::process_length}, stream);

    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();

    XCTAssertEqual(events.size(), 8);

    auto event_iterator = events.begin();

    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::acos(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::acos(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::acos(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::acos(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::acos(test::linear_input_data[5]), 0.01);
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
}

- (void)test_atan {
    channel_index_t const ch_idx = 8;

    auto module = test::make_number_module<double>(math1::kind::atan, ch_idx);
    auto stream =
        test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);

    module.process(time::range{0, test::process_length}, stream);

    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();

    XCTAssertEqual(events.size(), 8);

    auto event_iterator = events.begin();

    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::atan(test::linear_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::atan(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::atan(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::atan(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::atan(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::atan(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::atan(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::atan(test::linear_input_data[7]), 0.01);
}

- (void)test_sinh {
    channel_index_t const ch_idx = 40;

    auto module = test::make_number_module<double>(math1::kind::sinh, ch_idx);
    auto stream =
        test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);

    module.process(time::range{0, test::process_length}, stream);

    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();

    auto event_iterator = events.begin();

    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sinh(test::linear_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sinh(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sinh(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sinh(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sinh(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sinh(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sinh(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sinh(test::linear_input_data[7]), 0.01);
}

- (void)test_cosh {
    channel_index_t const ch_idx = 41;

    auto module = test::make_number_module<double>(math1::kind::cosh, ch_idx);
    auto stream =
        test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);

    module.process(time::range{0, test::process_length}, stream);

    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();

    auto event_iterator = events.begin();

    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cosh(test::linear_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cosh(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cosh(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cosh(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cosh(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cosh(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cosh(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cosh(test::linear_input_data[7]), 0.01);
}

- (void)test_tanh {
    channel_index_t const ch_idx = 42;

    auto module = test::make_number_module<double>(math1::kind::tanh, ch_idx);
    auto stream =
        test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);

    module.process(time::range{0, test::process_length}, stream);

    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();

    auto event_iterator = events.begin();

    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::tanh(test::linear_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::tanh(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::tanh(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::tanh(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::tanh(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::tanh(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::tanh(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::tanh(test::linear_input_data[7]), 0.01);
}

- (void)test_asinh {
    channel_index_t const ch_idx = 43;

    auto module = test::make_number_module<double>(math1::kind::asinh, ch_idx);
    auto stream =
        test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);

    module.process(time::range{0, test::process_length}, stream);

    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();

    auto event_iterator = events.begin();

    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::asinh(test::linear_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::asinh(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::asinh(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::asinh(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::asinh(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::asinh(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::asinh(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::asinh(test::linear_input_data[7]), 0.01);
}

- (void)test_acosh {
    channel_index_t const ch_idx = 44;

    auto module = test::make_number_module<double>(math1::kind::acosh, ch_idx);
    auto stream =
        test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);

    module.process(time::range{0, test::process_length}, stream);

    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();

    auto event_iterator = events.begin();

    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::acosh(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::acosh(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::acosh(test::linear_input_data[7]), 0.01);
}

- (void)test_atanh {
    channel_index_t const ch_idx = 45;

    auto module = test::make_number_module<double>(math1::kind::atanh, ch_idx);
    auto stream =
        test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);

    module.process(time::range{0, test::process_length}, stream);

    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();

    auto event_iterator = events.begin();

    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::atanh(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::atanh(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::atanh(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::atanh(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::atanh(test::linear_input_data[5]), 0.01);
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
}

- (void)test_exp {
    channel_index_t const ch_idx = 20;
    
    auto module = test::make_number_module<double>(math1::kind::exp, ch_idx);
    auto stream =
    test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();
    
    auto event_iterator = events.begin();
    
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::exp(test::linear_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::exp(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::exp(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::exp(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::exp(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::exp(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::exp(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::exp(test::linear_input_data[7]), 0.01);
}

- (void)test_exp2 {
    channel_index_t const ch_idx = 21;
    
    auto module = test::make_number_module<double>(math1::kind::exp2, ch_idx);
    auto stream =
    test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();
    
    auto event_iterator = events.begin();
    
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::exp2(test::linear_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::exp2(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::exp2(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::exp2(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::exp2(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::exp2(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::exp2(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::exp2(test::linear_input_data[7]), 0.01);
}

- (void)test_expm1 {
    channel_index_t const ch_idx = 22;
    
    auto module = test::make_number_module<double>(math1::kind::expm1, ch_idx);
    auto stream =
    test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();
    
    auto event_iterator = events.begin();
    
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::expm1(test::linear_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::expm1(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::expm1(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::expm1(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::expm1(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::expm1(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::expm1(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::expm1(test::linear_input_data[7]), 0.01);
}

- (void)test_log {
    channel_index_t const ch_idx = 23;
    
    auto module = test::make_number_module<double>(math1::kind::log, ch_idx);
    auto stream =
    test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();
    
    auto event_iterator = events.begin();
    
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log(test::linear_input_data[7]), 0.01);
}

- (void)test_log10 {
    channel_index_t const ch_idx = 24;
    
    auto module = test::make_number_module<double>(math1::kind::log10, ch_idx);
    auto stream =
    test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();
    
    auto event_iterator = events.begin();
    
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log10(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log10(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log10(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log10(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log10(test::linear_input_data[7]), 0.01);
}

- (void)test_log1p {
    channel_index_t const ch_idx = 25;
    
    auto module = test::make_number_module<double>(math1::kind::log1p, ch_idx);
    auto stream =
    test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();
    
    auto event_iterator = events.begin();
    
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log1p(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log1p(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log1p(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log1p(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log1p(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log1p(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log1p(test::linear_input_data[7]), 0.01);
}

- (void)test_log2 {
    channel_index_t const ch_idx = 26;
    
    auto module = test::make_number_module<double>(math1::kind::log2, ch_idx);
    auto stream =
    test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();
    
    auto event_iterator = events.begin();
    
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log2(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log2(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log2(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log2(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::log2(test::linear_input_data[7]), 0.01);
}

- (void)test_sqrt {
    channel_index_t const ch_idx = 9;
    
    auto module = test::make_number_module<double>(math1::kind::sqrt, ch_idx);
    auto stream =
    test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();
    
    auto event_iterator = events.begin();
    
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertTrue(std::isnan((event_iterator++)->second.get<double>()));
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sqrt(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sqrt(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sqrt(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sqrt(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::sqrt(test::linear_input_data[7]), 0.01);
}

- (void)test_cbrt {
    channel_index_t const ch_idx = 10;
    
    auto module = test::make_number_module<double>(math1::kind::cbrt, ch_idx);
    auto stream =
    test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();
    
    auto event_iterator = events.begin();
    
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cbrt(test::linear_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cbrt(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cbrt(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cbrt(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cbrt(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cbrt(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cbrt(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::cbrt(test::linear_input_data[7]), 0.01);
}

- (void)test_abs {
    channel_index_t const ch_idx = 11;
    
    auto module = test::make_number_module<double>(math1::kind::abs, ch_idx);
    auto stream =
    test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();
    
    auto event_iterator = events.begin();
    
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), 1.5, 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), 1.0, 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), 0.5, 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), 0.0, 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), 0.5, 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), 1.0, 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), 1.5, 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), 2.0, 0.01);
}

- (void)test_ceil {
    channel_index_t const ch_idx = 30;
    
    auto module = test::make_number_module<double>(math1::kind::ceil, ch_idx);
    auto stream =
    test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();
    
    auto event_iterator = events.begin();
    
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::ceil(test::linear_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::ceil(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::ceil(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::ceil(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::ceil(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::ceil(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::ceil(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::ceil(test::linear_input_data[7]), 0.01);
}

- (void)test_floor {
    channel_index_t const ch_idx = 31;
    
    auto module = test::make_number_module<double>(math1::kind::floor, ch_idx);
    auto stream =
    test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();
    
    auto event_iterator = events.begin();
    
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::floor(test::linear_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::floor(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::floor(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::floor(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::floor(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::floor(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::floor(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::floor(test::linear_input_data[7]), 0.01);
}

- (void)test_trunc {
    channel_index_t const ch_idx = 32;
    
    auto module = test::make_number_module<double>(math1::kind::trunc, ch_idx);
    auto stream =
    test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();
    
    auto event_iterator = events.begin();
    
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::trunc(test::linear_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::trunc(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::trunc(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::trunc(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::trunc(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::trunc(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::trunc(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::trunc(test::linear_input_data[7]), 0.01);
}

- (void)test_round {
    channel_index_t const ch_idx = 33;
    
    auto module = test::make_number_module<double>(math1::kind::round, ch_idx);
    auto stream =
    test::make_number_stream<double>(test::linear_input_data, time::range{0, test::process_length}, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const events = stream.channel(ch_idx).filtered_events<double, number_event>();
    
    auto event_iterator = events.begin();
    
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::round(test::linear_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::round(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::round(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::round(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::round(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::round(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::round(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy((event_iterator++)->second.get<double>(), std::round(test::linear_input_data[7]), 0.01);
}

@end
