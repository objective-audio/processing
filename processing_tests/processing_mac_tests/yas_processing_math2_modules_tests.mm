//
//  yas_processing_math2_modules_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;
using namespace yas::processing;

namespace yas {
    namespace test {
        template <typename T>
        static module make_module(math1::kind const kind, channel_index_t const ch_idx) {
            auto module = make_signal_module<T>(kind);
            module.connect_output(to_connector_index(math1::output_key::out), ch_idx);
            module.connect_input(to_connector_index(math1::input_key::in), ch_idx);
            
            return module;
        }
        
        template <typename T>
        static stream make_stream(time::range const time_range,
                                  T const *const left_data,
                                  time::range const left_time_range,
                                  channel_index_t const left_ch_idx,
                                  T const *const right_data,
                                  time::range const right_time_range,
                                  channel_index_t const right_ch_idx) {
            stream stream{sync_source{1, time_range.length}};
            
            {
                auto &channel = stream.add_channel(left_ch_idx);
                
                signal_event signal = make_signal_event<T>(left_time_range.length);
                auto *out_data = signal.data<T>();
                auto each = make_fast_each(out_data, left_time_range.length);
                while (yas_fast_each_next(each)) {
                    yas_fast_each_value(each) = left_data[yas_fast_each_index(each)];
                }
                
                channel.insert_event(processing::time{left_time_range}, std::move(signal));
            }
            
            {
                auto &channel = stream.add_channel(right_ch_idx);
                
                signal_event signal = make_signal_event<T>(right_time_range.length);
                auto *out_data = signal.data<T>();
                auto each = make_fast_each(out_data, right_time_range.length);
                while (yas_fast_each_next(each)) {
                    yas_fast_each_value(each) = right_data[yas_fast_each_index(each)];
                }
                
                channel.insert_event(processing::time{right_time_range}, std::move(signal));
            }
            
            return stream;
        }
    }
}

@interface yas_processing_math2_modules_tests : XCTestCase

@end

@implementation yas_processing_math2_modules_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create_signal_modules {
    auto plus_module = make_signal_module<int16_t>(math2::kind::plus);
    XCTAssertTrue(plus_module);

    auto minus_module = make_signal_module<int16_t>(math2::kind::minus);
    XCTAssertTrue(minus_module);

    auto multiply_module = make_signal_module<int16_t>(math2::kind::multiply);
    XCTAssertTrue(multiply_module);

    auto divide_module = make_signal_module<int16_t>(math2::kind::divide);
    XCTAssertTrue(divide_module);
}

- (void)test_plus_process {
    length_t const process_length = 5;
    
    int16_t const left_data[2] = {
        1, 1
    };
    
    int16_t const right_data[2] = {
        10, 10
    };
    
    auto stream = test::make_stream(time::range{0, process_length}, left_data, time::range{1, 2}, 0, right_data, time::range{2, 2}, 1);

    auto calc_module = make_signal_module<int16_t>(math2::kind::plus);
    calc_module.connect_input(to_connector_index(math2::input_key::left), 0);
    calc_module.connect_input(to_connector_index(math2::input_key::right), 1);
    calc_module.connect_output(to_connector_index(constant::output_key::out), 2);

    calc_module.process({0, process_length}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &events = stream.channel(2).events();

    XCTAssertEqual(events.size(), 1);

    auto const signal = cast<signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<int16_t>();

    XCTAssertEqual(vec.size(), process_length);
    XCTAssertEqual(vec[0], 0);
    XCTAssertEqual(vec[1], 1);
    XCTAssertEqual(vec[2], 11);
    XCTAssertEqual(vec[3], 10);
    XCTAssertEqual(vec[4], 0);
}

- (void)test_minus_process {
    length_t const process_length = 5;
    
    int16_t const left_data[2] = {
        100, 100
    };
    
    int16_t const right_data[2] = {
        10, 10
    };
    
    auto stream = test::make_stream(time::range{0, process_length}, left_data, time::range{1, 2}, 0, right_data, time::range{2, 2}, 1);

    auto calc_module = make_signal_module<int16_t>(math2::kind::minus);
    calc_module.connect_input(to_connector_index(math2::input_key::left), 0);
    calc_module.connect_input(to_connector_index(math2::input_key::right), 1);
    calc_module.connect_output(to_connector_index(constant::output_key::out), 2);

    calc_module.process({0, 5}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &events = stream.channel(2).events();

    XCTAssertEqual(events.size(), 1);

    auto const &signal = cast<signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<int16_t>();

    XCTAssertEqual(vec.size(), 5);
    XCTAssertEqual(vec[0], 0);
    XCTAssertEqual(vec[1], 100);
    XCTAssertEqual(vec[2], 90);
    XCTAssertEqual(vec[3], -10);
    XCTAssertEqual(vec[4], 0);
}

- (void)test_multiply_process {
    length_t const process_length = 5;
    
    int16_t const left_data[2] = {
        2, 2
    };
    
    int16_t const right_data[2] = {
        4, 4
    };
    
    auto stream = test::make_stream(time::range{0, process_length}, left_data, time::range{1, 2}, 0, right_data, time::range{2, 2}, 1);

    auto calc_module = make_signal_module<int16_t>(math2::kind::multiply);
    calc_module.connect_input(to_connector_index(math2::input_key::left), 0);
    calc_module.connect_input(to_connector_index(math2::input_key::right), 1);
    calc_module.connect_output(to_connector_index(constant::output_key::out), 2);

    calc_module.process({0, 5}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &events = stream.channel(2).events();

    XCTAssertEqual(events.size(), 1);

    auto const &signal = cast<signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<int16_t>();

    XCTAssertEqual(vec.size(), 5);
    XCTAssertEqual(vec[0], 0);
    XCTAssertEqual(vec[1], 0);
    XCTAssertEqual(vec[2], 8);
    XCTAssertEqual(vec[3], 0);
    XCTAssertEqual(vec[4], 0);
}

- (void)test_divide_process {
    length_t const process_length = 5;
    
    int16_t const left_data[2] = {
        4, 4
    };
    
    int16_t const right_data[2] = {
        2, 2
    };
    
    auto stream = test::make_stream(time::range{0, process_length}, left_data, time::range{1, 2}, 0, right_data, time::range{2, 2}, 1);

    auto calc_module = make_signal_module<int16_t>(math2::kind::divide);
    calc_module.connect_input(to_connector_index(math2::input_key::left), 0);
    calc_module.connect_input(to_connector_index(math2::input_key::right), 1);
    calc_module.connect_output(to_connector_index(constant::output_key::out), 2);

    calc_module.process({0, 5}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &events = stream.channel(2).events();

    XCTAssertEqual(events.size(), 1);

    auto const &signal = cast<signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<int16_t>();

    XCTAssertEqual(vec.size(), 5);
    XCTAssertEqual(vec[0], 0);
    XCTAssertEqual(vec[1], 0);
    XCTAssertEqual(vec[2], 2);
    XCTAssertEqual(vec[3], 0);
    XCTAssertEqual(vec[4], 0);
}

- (void)test_atan2 {
    length_t const process_length = 9;
    
    double const left_data[6] = {
        1.0, 1.0, 1.0, -1.0, -1.0, -1.0
    };
    
    double const right_data[6] = {
        1.0, -1.0, 1.0, -1.0, 1.0, -1.0
    };
    
    auto stream = test::make_stream(time::range{0, process_length}, left_data, time::range{1, 6}, 0, right_data, time::range{2, 6}, 1);
    
    auto calc_module = make_signal_module<double>(math2::kind::atan2);
    calc_module.connect_input(to_connector_index(math2::input_key::left), 0);
    calc_module.connect_input(to_connector_index(math2::input_key::right), 1);
    calc_module.connect_output(to_connector_index(constant::output_key::out), 2);
    
    calc_module.process({0, process_length}, stream);

    XCTAssertTrue(stream.has_channel(2));
    
    auto const &events = stream.channel(2).events();
    
    XCTAssertEqual(events.size(), 1);
    
    auto const &signal = cast<signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<double>();
    
    XCTAssertEqual(vec.size(), process_length);
    XCTAssertEqual(vec[0], std::atan2(0.0, 0.0));
    XCTAssertEqual(vec[1], std::atan2(1.0, 0.0));
    XCTAssertEqual(vec[2], std::atan2(1.0, 1.0));
    XCTAssertEqual(vec[3], std::atan2(1.0, -1.0));
    XCTAssertEqual(vec[4], std::atan2(-1.0, 1.0));
    XCTAssertEqual(vec[5], std::atan2(-1.0, -1.0));
    XCTAssertEqual(vec[6], std::atan2(-1.0, 1.0));
    XCTAssertEqual(vec[7], std::atan2(0.0, -1.0));
    XCTAssertEqual(vec[8], std::atan2(0.0, 0.0));
}

- (void)test_pow {
    length_t const process_length = 5;
    
    double const left_data[2] = {
        2.0, 2.0
    };
    
    double const right_data[2] = {
        4.0, 4.0
    };
    
    auto stream = test::make_stream(time::range{0, process_length}, left_data, time::range{1, 2}, 0, right_data, time::range{2, 2}, 1);
    
    auto calc_module = make_signal_module<double>(math2::kind::pow);
    calc_module.connect_input(to_connector_index(math2::input_key::left), 0);
    calc_module.connect_input(to_connector_index(math2::input_key::right), 1);
    calc_module.connect_output(to_connector_index(constant::output_key::out), 2);
    
    calc_module.process({0, 5}, stream);
    
    XCTAssertTrue(stream.has_channel(2));
    
    auto const &events = stream.channel(2).events();
    
    XCTAssertEqual(events.size(), 1);
    
    auto const &signal = cast<signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<double>();
    
    XCTAssertEqual(vec.size(), 5);
    XCTAssertEqual(vec[0], std::pow(0.0, 0.0));
    XCTAssertEqual(vec[1], std::pow(2.0, 0.0));
    XCTAssertEqual(vec[2], std::pow(2.0, 4.0));
    XCTAssertEqual(vec[3], std::pow(0.0, 4.0));
    XCTAssertEqual(vec[4], std::pow(0.0, 0.0));
}

- (void)test_hypot {
    length_t const process_length = 5;
    
    double const left_data[2] = {
        1.0, 1.0
    };
    
    double const right_data[2] = {
        3.0, 3.0
    };
    
    auto stream = test::make_stream(time::range{0, process_length}, left_data, time::range{1, 2}, 0, right_data, time::range{2, 2}, 1);
    
    auto calc_module = make_signal_module<double>(math2::kind::hypot);
    calc_module.connect_input(to_connector_index(math2::input_key::left), 0);
    calc_module.connect_input(to_connector_index(math2::input_key::right), 1);
    calc_module.connect_output(to_connector_index(constant::output_key::out), 2);
    
    calc_module.process({0, 5}, stream);
    
    XCTAssertTrue(stream.has_channel(2));
    
    auto const &events = stream.channel(2).events();
    
    XCTAssertEqual(events.size(), 1);
    
    auto const &signal = cast<signal_event>((*events.begin()).second);
    auto const &vec = signal.vector<double>();
    
    XCTAssertEqual(vec.size(), 5);
    XCTAssertEqual(vec[0], std::hypot(0.0, 0.0));
    XCTAssertEqual(vec[1], std::hypot(1.0, 0.0));
    XCTAssertEqual(vec[2], std::hypot(1.0, 3.0));
    XCTAssertEqual(vec[3], std::hypot(0.0, 3.0));
    XCTAssertEqual(vec[4], std::hypot(0.0, 0.0));
}

@end
