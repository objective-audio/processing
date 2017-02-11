//
//  yas_processing_routing_modules_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing_test_utils.h"
#import "yas_boolean.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_routing_modules_tests : XCTestCase

@end

@implementation yas_processing_routing_modules_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_make_signal_module {
    XCTAssertTrue(make_signal_module<double>(routing::kind::move));
    XCTAssertTrue(make_signal_module<float>(routing::kind::move));
    XCTAssertTrue(make_signal_module<int64_t>(routing::kind::move));
    XCTAssertTrue(make_signal_module<int32_t>(routing::kind::move));
    XCTAssertTrue(make_signal_module<int16_t>(routing::kind::move));
    XCTAssertTrue(make_signal_module<int8_t>(routing::kind::move));
    XCTAssertTrue(make_signal_module<uint64_t>(routing::kind::move));
    XCTAssertTrue(make_signal_module<uint32_t>(routing::kind::move));
    XCTAssertTrue(make_signal_module<uint16_t>(routing::kind::move));
    XCTAssertTrue(make_signal_module<uint8_t>(routing::kind::move));
    XCTAssertTrue(make_signal_module<boolean>(routing::kind::move));

    XCTAssertTrue(make_signal_module<double>(routing::kind::copy));
    XCTAssertTrue(make_signal_module<float>(routing::kind::copy));
    XCTAssertTrue(make_signal_module<int64_t>(routing::kind::copy));
    XCTAssertTrue(make_signal_module<int32_t>(routing::kind::copy));
    XCTAssertTrue(make_signal_module<int16_t>(routing::kind::copy));
    XCTAssertTrue(make_signal_module<int8_t>(routing::kind::copy));
    XCTAssertTrue(make_signal_module<uint64_t>(routing::kind::copy));
    XCTAssertTrue(make_signal_module<uint32_t>(routing::kind::copy));
    XCTAssertTrue(make_signal_module<uint16_t>(routing::kind::copy));
    XCTAssertTrue(make_signal_module<uint8_t>(routing::kind::copy));
    XCTAssertTrue(make_signal_module<boolean>(routing::kind::copy));
}

- (void)test_move_process {
    length_t const process_length = 2;
    channel_index_t const input_ch_idx = 0;
    channel_index_t const output_ch_idx = 1;

    int8_t data[process_length] = {1, 2};

    auto module = make_signal_module<int8_t>(routing::kind::move);
    module.connect_input(to_connector_index(routing::input::value), input_ch_idx);
    module.connect_output(to_connector_index(routing::output::value), output_ch_idx);

    auto stream = test::make_stream(time::range{0, process_length}, data, time::range{0, process_length}, 0);

    module.process(time::range{0, process_length}, stream);

    auto const &input_events = stream.channel(input_ch_idx).events();
    XCTAssertEqual(input_events.size(), 0);

    auto const &output_events = stream.channel(output_ch_idx).events();
    XCTAssertEqual(output_events.size(), 1);

    auto const signal = cast<processing::signal_event>(output_events.begin()->second);
    auto const &vec = signal.vector<int8_t>();

    XCTAssertEqual(vec[0], 1);
    XCTAssertEqual(vec[1], 2);
}

- (void)test_move_process_part {
    length_t const process_length = 2;
    channel_index_t const input_ch_idx = 0;
    channel_index_t const output_ch_idx = 1;

    int8_t data[process_length] = {1, 2};

    auto module = make_signal_module<int8_t>(routing::kind::move);
    module.connect_input(to_connector_index(routing::input::value), input_ch_idx);
    module.connect_output(to_connector_index(routing::output::value), output_ch_idx);

    auto stream = test::make_stream(time::range{0, process_length}, data, time::range{0, process_length}, 0);

    module.process(time::range{1, 1}, stream);

    auto const &unprocessed_events = stream.channel(input_ch_idx).events();
    XCTAssertEqual(unprocessed_events.size(), 1);

    auto const unprocessed_time = unprocessed_events.begin()->first;
    auto const unprocessed_signal = cast<processing::signal_event>(unprocessed_events.begin()->second);
    auto const &unprocessed_vec = unprocessed_signal.vector<int8_t>();

    XCTAssertEqual(unprocessed_time.get<time::range>(), time::range(0, 1));
    XCTAssertEqual(unprocessed_vec[0], 1);

    auto const &processed_events = stream.channel(output_ch_idx).events();
    XCTAssertEqual(processed_events.size(), 1);

    auto const processed_time = processed_events.begin()->first;
    auto const processed_signal = cast<processing::signal_event>(processed_events.begin()->second);
    auto const &processed_vec = processed_signal.vector<int8_t>();

    XCTAssertEqual(processed_time.get<time::range>(), time::range(1, 1));
    XCTAssertEqual(processed_vec[0], 2);
}

- (void)test_copy_process {
    length_t const process_length = 2;
    channel_index_t const input_ch_idx = 0;
    channel_index_t const output_ch_idx = 1;

    int8_t data[process_length] = {1, 2};

    auto module = make_signal_module<int8_t>(routing::kind::copy);
    module.connect_input(to_connector_index(routing::input::value), input_ch_idx);
    module.connect_output(to_connector_index(routing::output::value), output_ch_idx);

    auto stream = test::make_stream(time::range{0, process_length}, data, time::range{0, process_length}, 0);

    module.process(time::range{0, process_length}, stream);

    auto const &input_events = stream.channel(input_ch_idx).events();
    XCTAssertEqual(input_events.size(), 1);

    auto const input_signal = cast<processing::signal_event>(input_events.begin()->second);
    auto const &input_vec = input_signal.vector<int8_t>();

    XCTAssertEqual(input_vec[0], 1);
    XCTAssertEqual(input_vec[1], 2);

    auto const &output_events = stream.channel(output_ch_idx).events();
    XCTAssertEqual(output_events.size(), 1);

    auto const output_signal = cast<processing::signal_event>(output_events.begin()->second);
    auto const &output_vec = output_signal.vector<int8_t>();

    XCTAssertEqual(output_vec[0], 1);
    XCTAssertEqual(output_vec[1], 2);
}

- (void)test_copy_process_part {
    length_t const process_length = 2;
    channel_index_t const input_ch_idx = 0;
    channel_index_t const output_ch_idx = 1;
    
    int8_t data[process_length] = {1, 2};
    
    auto module = make_signal_module<int8_t>(routing::kind::copy);
    module.connect_input(to_connector_index(routing::input::value), input_ch_idx);
    module.connect_output(to_connector_index(routing::output::value), output_ch_idx);
    
    auto stream = test::make_stream(time::range{0, process_length}, data, time::range{0, process_length}, 0);
    
    module.process(time::range{1, 1}, stream);
    
    auto const &input_events = stream.channel(input_ch_idx).events();
    XCTAssertEqual(input_events.size(), 1);
    
    auto const input_time = input_events.begin()->first;
    auto const input_signal = cast<processing::signal_event>(input_events.begin()->second);
    auto const &input_vec = input_signal.vector<int8_t>();
    
    XCTAssertEqual(input_time.get<time::range>(), time::range(0, process_length));
    XCTAssertEqual(input_vec[0], 1);
    XCTAssertEqual(input_vec[1], 2);
    
    auto const &output_events = stream.channel(output_ch_idx).events();
    XCTAssertEqual(output_events.size(), 1);
    
    auto const output_time = output_events.begin()->first;
    auto const output_signal = cast<processing::signal_event>(output_events.begin()->second);
    auto const &output_vec = output_signal.vector<int8_t>();
    
    XCTAssertEqual(output_time.get<time::range>(), time::range(1, 1));
    XCTAssertEqual(output_vec[0], 2);
}

@end
