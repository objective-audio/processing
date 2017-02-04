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

- (void)test_process_signal_diff_channel {
    auto constant_module1 = constant::make_signal_module(int8_t(1));
    constant_module1.connect_output(to_connector_index(constant::output::value), 0);

    auto constant_module2 = constant::make_signal_module(int8_t(2));
    constant_module2.connect_output(to_connector_index(constant::output::value), 0);

    auto cast_module = cast::make_signal_module<int8_t, float>();
    cast_module.connect_input(to_connector_index(cast::input_key::value), 0);
    cast_module.connect_output(to_connector_index(cast::output::value), 1);

    stream stream{sync_source{1, 2}};

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

- (void)test_process_signal_same_channel {
    auto constant_module1 = constant::make_signal_module(int8_t(32));
    constant_module1.connect_output(to_connector_index(constant::output::value), 2);

    auto constant_module2 = constant::make_signal_module(int8_t(64));
    constant_module2.connect_output(to_connector_index(constant::output::value), 2);

    auto cast_module = cast::make_signal_module<int8_t, double>();
    cast_module.connect_input(to_connector_index(cast::input_key::value), 2);
    cast_module.connect_output(to_connector_index(cast::output::value), 2);

    stream stream{sync_source{1, 2}};

    constant_module1.process({0, 1}, stream);
    constant_module2.process({1, 1}, stream);
    cast_module.process({0, 2}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &channel = stream.channel(2);
    XCTAssertEqual(channel.events().size(), 1);

    auto const filtered_events = channel.filtered_events<double, processing::signal_event>();
    XCTAssertEqual(filtered_events.size(), 1);

    auto const &pair = *filtered_events.begin();

    XCTAssertTrue((pair.first == time::range{0, 2}));
    auto const &signal = pair.second;
    XCTAssertTrue(signal.sample_type() == typeid(double));
    XCTAssertEqual(signal.size(), 2);
    auto const *data = signal.data<double>();
    XCTAssertEqual(data[0], 32.0);
    XCTAssertEqual(data[1], 64.0);
}

- (void)test_process_number_diff_channel {
    auto cast_module = cast::make_number_module<int8_t, float>();
    cast_module.connect_input(to_connector_index(cast::input_key::value), 0);
    cast_module.connect_output(to_connector_index(cast::output::value), 1);

    stream stream{sync_source{1, 2}};

    {
        auto &channel0 = stream.add_channel(0);
        channel0.insert_event(make_frame_time(0), number_event{int8_t(0)});
        channel0.insert_event(make_frame_time(1), number_event{int8_t(1)});
        channel0.insert_event(make_frame_time(2), number_event{int8_t(2)});
        channel0.insert_event(make_frame_time(0), number_event{int16_t(-1)});
        auto &channel1 = stream.add_channel(1);
        channel1.insert_event(make_frame_time(0), number_event{int8_t(10)});
        auto &channel2 = stream.add_channel(2);
        channel2.insert_event(make_frame_time(0), number_event{int8_t(20)});
    }

    cast_module.process({0, 2}, stream);

    XCTAssertTrue(stream.has_channel(0));
    auto const &channel0 = stream.channel(0);
    XCTAssertEqual(channel0.events().size(), 2);
    auto it = channel0.events().begin();
    XCTAssertEqual((*it).first, make_frame_time(0));
    XCTAssertEqual((*it).second, number_event{int16_t(-1)});
    ++it;
    XCTAssertEqual((*it).first, make_frame_time(2));
    XCTAssertEqual((*it).second, number_event{int8_t(2)});

    XCTAssertTrue(stream.has_channel(1));
    auto const &channel1 = stream.channel(1);
    XCTAssertEqual(channel1.events().size(), 3);
    it = channel1.events().begin();
    XCTAssertEqual((*it).first, make_frame_time(0));
    XCTAssertEqual((*it).second, number_event{int8_t(10)});
    ++it;
    XCTAssertEqual((*it).first, make_frame_time(0));
    XCTAssertEqual((*it).second, number_event{float(0.0f)});
    ++it;
    XCTAssertEqual((*it).first, make_frame_time(1));
    XCTAssertEqual((*it).second, number_event{float(1.0f)});

    XCTAssertTrue(stream.has_channel(2));
    auto const &channel2 = stream.channel(2);
    XCTAssertEqual(channel2.events().size(), 1);
    it = channel2.events().begin();
    XCTAssertEqual((*it).first, make_frame_time(0));
    XCTAssertEqual((*it).second, number_event{int8_t(20)});
}

- (void)test_process_number_same_channel {
    auto cast_module = cast::make_number_module<int32_t, double>();
    cast_module.connect_input(to_connector_index(cast::input_key::value), 3);
    cast_module.connect_output(to_connector_index(cast::output::value), 3);

    stream stream{sync_source{1, 2}};

    {
        auto &channel = stream.add_channel(3);
        channel.insert_event(make_frame_time(0), number_event{int32_t(0)});
        channel.insert_event(make_frame_time(1), number_event{int32_t(1)});
        channel.insert_event(make_frame_time(2), number_event{int32_t(2)});
        channel.insert_event(make_frame_time(0), number_event{int16_t(-1)});
    }

    cast_module.process({0, 2}, stream);

    XCTAssertTrue(stream.has_channel(3));
    auto const &channel = stream.channel(3);
    XCTAssertEqual(channel.events().size(), 4);

    auto it = channel.events().begin();
    XCTAssertEqual((*it).first, make_frame_time(0));
    XCTAssertEqual((*it).second, number_event{int16_t(-1)});
    ++it;
    XCTAssertEqual((*it).first, make_frame_time(0));
    XCTAssertEqual((*it).second, number_event{double(0.0)});
    ++it;
    XCTAssertEqual((*it).first, make_frame_time(1));
    XCTAssertEqual((*it).second, number_event{double(1.0)});
    ++it;
    XCTAssertEqual((*it).first, make_frame_time(2));
    XCTAssertEqual((*it).second, number_event{int32_t(2)});
}

@end
