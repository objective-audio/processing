//
//  yas_processing_math1_modules_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;
using namespace yas::processing;

namespace yas {
namespace test {
    static double constexpr two_pi = 2.0 * M_PI;
    
    static std::size_t constexpr process_length = 8;
    
    static double constexpr radian_input_data[process_length] = {
        0.0,
        0.25 * two_pi,
        0.5 * two_pi,
        0.75 * two_pi,
        1.0 * two_pi,
        1.25 * two_pi,
        1.5 * two_pi,
        1.75 * two_pi,
    };
    
    static double constexpr linear_input_data[process_length] {
        -1.5,
        -1.0,
        -0.5,
        0.0,
        0.5,
        1.0,
        1.5,
        2.0,
    };
    
    static module make_module(math1::kind const kind, channel_index_t const ch_idx) {
        auto module = make_signal_module<double>(kind);
        module.connect_output(to_connector_index(math1::output_key::out), ch_idx);
        module.connect_input(to_connector_index(math1::input_key::in), ch_idx);
        
        return module;
    }
    
    static stream make_stream(double const *const data, channel_index_t const ch_idx) {
        stream stream{sync_source{1, process_length}};
        
        auto &channel = stream.add_channel(ch_idx);
        
        auto phase_signal = make_signal_event<double>(process_length);
        auto *phase_data = phase_signal.data<double>();
        auto phase_each = make_fast_each(phase_data, process_length);
        while (yas_fast_each_next(phase_each)) {
            yas_fast_each_value(phase_each) = data[yas_fast_each_index(phase_each)];
        }
        
        channel.insert_event(make_range_time(0, process_length), std::move(phase_signal));
        
        return stream;
    }
}
}

@interface yas_processing_math1_modules_tests : XCTestCase

@end

@implementation yas_processing_math1_modules_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_signal_module {
    auto sin_module = make_signal_module<double>(math1::kind::sin);
    XCTAssertTrue(sin_module);
    
    auto cos_module = make_signal_module<double>(math1::kind::cos);
    XCTAssertTrue(cos_module);
    
    auto tan_module = make_signal_module<double>(math1::kind::tan);
    XCTAssertTrue(tan_module);
    
    auto asin_module = make_signal_module<double>(math1::kind::asin);
    XCTAssertTrue(asin_module);
    
    auto acos_module = make_signal_module<double>(math1::kind::acos);
    XCTAssertTrue(acos_module);
    
    auto atan_module = make_signal_module<double>(math1::kind::atan);
    XCTAssertTrue(atan_module);
}

- (void)test_sin{
    channel_index_t const ch_idx = 3;

    auto module = test::make_module(math1::kind::sin, ch_idx);
    auto stream = test::make_stream(test::radian_input_data, ch_idx);

    module.process(time::range{0, test::process_length}, stream);

    auto const &events = stream.channel(ch_idx).events();
    auto const signal = cast<processing::signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<double>();
    
    XCTAssertEqualWithAccuracy(vec[0], std::sin(test::radian_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy(vec[1], std::sin(test::radian_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy(vec[2], std::sin(test::radian_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy(vec[3], std::sin(test::radian_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy(vec[4], std::sin(test::radian_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy(vec[5], std::sin(test::radian_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy(vec[6], std::sin(test::radian_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy(vec[7], std::sin(test::radian_input_data[7]), 0.01);
}

- (void)test_cos {
    channel_index_t const ch_idx = 4;
    
    auto module = test::make_module(math1::kind::cos, ch_idx);
    auto stream = test::make_stream(test::radian_input_data, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const &events = stream.channel(ch_idx).events();
    auto const signal = cast<processing::signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<double>();
    
    XCTAssertEqualWithAccuracy(vec[0], std::cos(test::radian_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy(vec[1], std::cos(test::radian_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy(vec[2], std::cos(test::radian_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy(vec[3], std::cos(test::radian_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy(vec[4], std::cos(test::radian_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy(vec[5], std::cos(test::radian_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy(vec[6], std::cos(test::radian_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy(vec[7], std::cos(test::radian_input_data[7]), 0.01);
}

- (void)test_tan {
    channel_index_t const ch_idx = 5;
    
    auto module = test::make_module(math1::kind::tan, ch_idx);
    auto stream = test::make_stream(test::radian_input_data, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const &events = stream.channel(ch_idx).events();
    auto const signal = cast<processing::signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<double>();
    
    XCTAssertEqualWithAccuracy(vec[0], std::tan(test::radian_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy(vec[1], std::tan(test::radian_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy(vec[2], std::tan(test::radian_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy(vec[3], std::tan(test::radian_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy(vec[4], std::tan(test::radian_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy(vec[5], std::tan(test::radian_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy(vec[6], std::tan(test::radian_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy(vec[7], std::tan(test::radian_input_data[7]), 0.01);
}

- (void)test_asin {
    channel_index_t const ch_idx = 6;
    
    auto module = test::make_module(math1::kind::asin, ch_idx);
    auto stream = test::make_stream(test::linear_input_data, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const &events = stream.channel(ch_idx).events();
    auto const signal = cast<processing::signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<double>();
    
    XCTAssertTrue(std::isnan(vec[0]));
    XCTAssertEqualWithAccuracy(vec[1], std::asin(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy(vec[2], std::asin(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy(vec[3], std::asin(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy(vec[4], std::asin(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy(vec[5], std::asin(test::linear_input_data[5]), 0.01);
    XCTAssertTrue(std::isnan(vec[6]));
    XCTAssertTrue(std::isnan(vec[7]));
}

- (void)test_acos {
    channel_index_t const ch_idx = 7;
    
    auto module = test::make_module(math1::kind::acos, ch_idx);
    auto stream = test::make_stream(test::linear_input_data, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const &events = stream.channel(ch_idx).events();
    auto const signal = cast<processing::signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<double>();
    
    XCTAssertTrue(std::isnan(vec[0]));
    XCTAssertEqualWithAccuracy(vec[1], std::acos(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy(vec[2], std::acos(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy(vec[3], std::acos(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy(vec[4], std::acos(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy(vec[5], std::acos(test::linear_input_data[5]), 0.01);
    XCTAssertTrue(std::isnan(vec[6]));
    XCTAssertTrue(std::isnan(vec[7]));
}

- (void)test_atan {
    channel_index_t const ch_idx = 8;
    
    auto module = test::make_module(math1::kind::atan, ch_idx);
    auto stream = test::make_stream(test::linear_input_data, ch_idx);
    
    module.process(time::range{0, test::process_length}, stream);
    
    auto const &events = stream.channel(ch_idx).events();
    auto const signal = cast<processing::signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<double>();
    
    XCTAssertEqualWithAccuracy(vec[0], std::atan(test::linear_input_data[0]), 0.01);
    XCTAssertEqualWithAccuracy(vec[1], std::atan(test::linear_input_data[1]), 0.01);
    XCTAssertEqualWithAccuracy(vec[2], std::atan(test::linear_input_data[2]), 0.01);
    XCTAssertEqualWithAccuracy(vec[3], std::atan(test::linear_input_data[3]), 0.01);
    XCTAssertEqualWithAccuracy(vec[4], std::atan(test::linear_input_data[4]), 0.01);
    XCTAssertEqualWithAccuracy(vec[5], std::atan(test::linear_input_data[5]), 0.01);
    XCTAssertEqualWithAccuracy(vec[6], std::atan(test::linear_input_data[6]), 0.01);
    XCTAssertEqualWithAccuracy(vec[7], std::atan(test::linear_input_data[7]), 0.01);
}

@end
