//
//  yas_processing_math2_number_modules_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"
#import "yas_fast_each.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_math2_number_modules_tests : XCTestCase

@end

@implementation yas_processing_math2_number_modules_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create_number_modules {
    XCTAssertTrue(make_number_module<float>(math2::kind::plus));
    XCTAssertTrue(make_number_module<float>(math2::kind::minus));
    XCTAssertTrue(make_number_module<float>(math2::kind::multiply));
    XCTAssertTrue(make_number_module<float>(math2::kind::divide));
    XCTAssertTrue(make_number_module<float>(math2::kind::atan2));
    XCTAssertTrue(make_number_module<float>(math2::kind::pow));
    XCTAssertTrue(make_number_module<float>(math2::kind::hypot));
}

- (void)test_out_of_range {
    length_t const process_length = 1;

    stream stream{sync_source{1, process_length}};

    auto &left_channel = stream.add_channel(0);
    left_channel.insert_event(make_frame_time(-1), number_event(int8_t(1)));
    left_channel.insert_event(make_frame_time(0), number_event(int8_t(2)));
    left_channel.insert_event(make_frame_time(1), number_event(int8_t(3)));

    auto module = make_number_module<int8_t>(math2::kind::plus);
    connect(module, math2::input::left, 0);
    connect(module, math2::output::result, 2);

    module.process({0, process_length}, stream);

    auto const &result_channel = stream.channel(2);

    XCTAssertEqual(result_channel.events().size(), 1);

    auto const &event_pair = *result_channel.events().cbegin();

    XCTAssertEqual(event_pair.first, make_frame_time(0));
}

- (void)test_overwrite {
    length_t const process_length = 1;

    stream stream{sync_source{1, process_length}};

    auto &left_channel = stream.add_channel(0);
    left_channel.insert_event(make_frame_time(0), number_event(int8_t(1)));

    auto &right_channel = stream.add_channel(1);
    right_channel.insert_event(make_frame_time(0), number_event(int8_t(10)));

    auto module = make_number_module<int8_t>(math2::kind::plus);
    connect(module, math2::input::left, 0);
    connect(module, math2::input::right, 1);
    connect(module, math2::output::result, 0);

    module.process({0, process_length}, stream);

    auto const &result_channel = stream.channel(0);

    XCTAssertEqual(result_channel.events().size(), 1);

    auto const &event_pair = *result_channel.events().cbegin();

    XCTAssertEqual(event_pair.first, make_frame_time(0));
    XCTAssertEqual(event_pair.second, number_event{int8_t(11)});
}

- (void)test_plus_process {
    length_t const process_length = 4;

    stream stream{sync_source{1, process_length}};

    auto &left_channel = stream.add_channel(0);
    left_channel.insert_event(make_frame_time(0), number_event(int8_t(1)));
    left_channel.insert_event(make_frame_time(3), number_event(int8_t(2)));

    auto &right_channel = stream.add_channel(1);
    right_channel.insert_event(make_frame_time(1), number_event(int8_t(10)));
    right_channel.insert_event(make_frame_time(3), number_event(int8_t(20)));

    auto module = make_number_module<int8_t>(math2::kind::plus);
    connect(module, math2::input::left, 0);
    connect(module, math2::input::right, 1);
    connect(module, math2::output::result, 2);

    module.process({0, process_length}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &result_channel = stream.channel(2);

    XCTAssertEqual(result_channel.events().size(), 3);

    auto event_iterator = result_channel.events().cbegin();

    XCTAssertEqual(event_iterator->first, make_frame_time(0));
    XCTAssertEqual(event_iterator->second, number_event(int8_t(1)));

    ++event_iterator;

    XCTAssertEqual(event_iterator->first, make_frame_time(1));
    XCTAssertEqual(event_iterator->second, number_event(int8_t(11)));

    ++event_iterator;

    XCTAssertEqual(event_iterator->first, make_frame_time(3));
    XCTAssertEqual(event_iterator->second, number_event(int8_t(22)));
}

- (void)test_minus_process {
    length_t const process_length = 1;

    stream stream{sync_source{1, process_length}};

    auto &left_channel = stream.add_channel(0);
    left_channel.insert_event(make_frame_time(0), number_event(int8_t(3)));

    auto &right_channel = stream.add_channel(1);
    right_channel.insert_event(make_frame_time(0), number_event(int8_t(2)));

    auto module = make_number_module<int8_t>(math2::kind::minus);
    connect(module, math2::input::left, 0);
    connect(module, math2::input::right, 1);
    connect(module, math2::output::result, 2);

    module.process({0, process_length}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &result_channel = stream.channel(2);

    XCTAssertEqual(result_channel.events().size(), 1);

    auto const &event_pair = *result_channel.events().cbegin();

    XCTAssertEqual(event_pair.first, make_frame_time(0));
    XCTAssertEqual(event_pair.second, number_event(int8_t(1)));
}

- (void)test_multiply_process {
    length_t const process_length = 1;

    stream stream{sync_source{1, process_length}};

    auto &left_channel = stream.add_channel(0);
    left_channel.insert_event(make_frame_time(0), number_event(int8_t(2)));

    auto &right_channel = stream.add_channel(1);
    right_channel.insert_event(make_frame_time(0), number_event(int8_t(4)));

    auto module = make_number_module<int8_t>(math2::kind::multiply);
    connect(module, math2::input::left, 0);
    connect(module, math2::input::right, 1);
    connect(module, math2::output::result, 2);

    module.process({0, process_length}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &result_channel = stream.channel(2);

    XCTAssertEqual(result_channel.events().size(), 1);

    auto const &event_pair = *result_channel.events().cbegin();

    XCTAssertEqual(event_pair.first, make_frame_time(0));
    XCTAssertEqual(event_pair.second, number_event(int8_t(8)));
}

- (void)test_divide_process {
    length_t const process_length = 1;

    stream stream{sync_source{1, process_length}};

    auto &left_channel = stream.add_channel(0);
    left_channel.insert_event(make_frame_time(0), number_event(int8_t(16)));

    auto &right_channel = stream.add_channel(1);
    right_channel.insert_event(make_frame_time(0), number_event(int8_t(8)));

    auto module = make_number_module<int8_t>(math2::kind::divide);
    connect(module, math2::input::left, 0);
    connect(module, math2::input::right, 1);
    connect(module, math2::output::result, 2);

    module.process({0, process_length}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &result_channel = stream.channel(2);

    XCTAssertEqual(result_channel.events().size(), 1);

    auto const &event_pair = *result_channel.events().cbegin();

    XCTAssertEqual(event_pair.first, make_frame_time(0));
    XCTAssertEqual(event_pair.second, number_event(int8_t(2)));
}

- (void)test_atan2_process {
    length_t const process_length = 8;
    double const left_data[process_length] = {0.0, 1.0, 1.0, 1.0, -1.0, -1.0, -1.0, 0.0};
    double const right_data[process_length] = {0.0, 0.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0};

    stream stream{sync_source{1, process_length}};

    auto &left_channel = stream.add_channel(0);
    auto left_each = make_each(process_length);
    while (yas_each_next(left_each)) {
        auto const &idx = yas_each_index(left_each);
        left_channel.insert_event(make_frame_time(idx), number_event(left_data[idx]));
    }

    auto &right_channel = stream.add_channel(1);
    auto right_each = make_each(process_length);
    while (yas_each_next(right_each)) {
        auto const &idx = yas_each_index(right_each);
        right_channel.insert_event(make_frame_time(idx), number_event(right_data[idx]));
    }

    auto module = make_number_module<double>(math2::kind::atan2);
    connect(module, math2::input::left, 0);
    connect(module, math2::input::right, 1);
    connect(module, math2::output::result, 2);

    module.process({0, process_length}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &result_channel = stream.channel(2);

    XCTAssertEqual(result_channel.events().size(), 8);

    auto event_iterator = result_channel.events().cbegin();

    XCTAssertEqual((event_iterator++)->second, number_event(std::atan2(0.0, 0.0)));
    XCTAssertEqual((event_iterator++)->second, number_event(std::atan2(1.0, 0.0)));
    XCTAssertEqual((event_iterator++)->second, number_event(std::atan2(1.0, 1.0)));
    XCTAssertEqual((event_iterator++)->second, number_event(std::atan2(1.0, -1.0)));
    XCTAssertEqual((event_iterator++)->second, number_event(std::atan2(-1.0, 1.0)));
    XCTAssertEqual((event_iterator++)->second, number_event(std::atan2(-1.0, -1.0)));
    XCTAssertEqual((event_iterator++)->second, number_event(std::atan2(-1.0, 1.0)));
    XCTAssertEqual((event_iterator++)->second, number_event(std::atan2(0.0, -1.0)));
}

- (void)test_pow_process {
    length_t const process_length = 4;
    double const left_data[process_length] = {0.0, 2.0, 2.0, 0.0};
    double const right_data[process_length] = {0.0, 0.0, 4.0, 4.0};

    stream stream{sync_source{1, process_length}};

    auto &left_channel = stream.add_channel(0);
    auto left_each = make_each(process_length);
    while (yas_each_next(left_each)) {
        auto const &idx = yas_each_index(left_each);
        left_channel.insert_event(make_frame_time(idx), number_event(left_data[idx]));
    }

    auto &right_channel = stream.add_channel(1);
    auto right_each = make_each(process_length);
    while (yas_each_next(right_each)) {
        auto const &idx = yas_each_index(right_each);
        right_channel.insert_event(make_frame_time(idx), number_event(right_data[idx]));
    }

    auto module = make_number_module<double>(math2::kind::pow);
    connect(module, math2::input::left, 0);
    connect(module, math2::input::right, 1);
    connect(module, math2::output::result, 2);

    module.process({0, process_length}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &result_channel = stream.channel(2);

    XCTAssertEqual(result_channel.events().size(), 4);

    auto event_iterator = result_channel.events().cbegin();

    XCTAssertEqual((event_iterator++)->second, number_event(std::pow(0.0, 0.0)));
    XCTAssertEqual((event_iterator++)->second, number_event(std::pow(2.0, 0.0)));
    XCTAssertEqual((event_iterator++)->second, number_event(std::pow(2.0, 4.0)));
    XCTAssertEqual((event_iterator++)->second, number_event(std::pow(0.0, 4.0)));
}

- (void)test_hypot_process {
    length_t const process_length = 4;
    double const left_data[process_length] = {0.0, 1.0, 1.0, 0.0};
    double const right_data[process_length] = {0.0, 0.0, 3.0, 3.0};

    stream stream{sync_source{1, process_length}};

    auto &left_channel = stream.add_channel(0);
    auto left_each = make_each(process_length);
    while (yas_each_next(left_each)) {
        auto const &idx = yas_each_index(left_each);
        left_channel.insert_event(make_frame_time(idx), number_event(left_data[idx]));
    }

    auto &right_channel = stream.add_channel(1);
    auto right_each = make_each(process_length);
    while (yas_each_next(right_each)) {
        auto const &idx = yas_each_index(right_each);
        right_channel.insert_event(make_frame_time(idx), number_event(right_data[idx]));
    }

    auto module = make_number_module<double>(math2::kind::hypot);
    connect(module, math2::input::left, 0);
    connect(module, math2::input::right, 1);
    connect(module, math2::output::result, 2);

    module.process({0, process_length}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const &result_channel = stream.channel(2);

    XCTAssertEqual(result_channel.events().size(), 4);

    auto event_iterator = result_channel.events().cbegin();

    XCTAssertEqual((event_iterator++)->second, number_event(std::hypot(0.0, 0.0)));
    XCTAssertEqual((event_iterator++)->second, number_event(std::hypot(1.0, 0.0)));
    XCTAssertEqual((event_iterator++)->second, number_event(std::hypot(1.0, 3.0)));
    XCTAssertEqual((event_iterator++)->second, number_event(std::hypot(0.0, 3.0)));
}

- (void)test_connect_input {
    auto module = make_number_module<double>(math2::kind::plus);
    connect(module, math2::input::left, 11);

    auto const &connectors = module.input_connectors();

    XCTAssertEqual(connectors.size(), 1);
    XCTAssertEqual(connectors.cbegin()->first, to_connector_index(math2::input::left));
    XCTAssertEqual(connectors.cbegin()->second.channel_index, 11);
}

- (void)test_connect_output {
    auto module = make_number_module<double>(math2::kind::minus);
    connect(module, math2::output::result, 2);

    auto const &connectors = module.output_connectors();

    XCTAssertEqual(connectors.size(), 1);
    XCTAssertEqual(connectors.cbegin()->first, to_connector_index(math2::output::result));
    XCTAssertEqual(connectors.cbegin()->second.channel_index, 2);
}

- (void)test_kind_to_string {
    XCTAssertEqual(to_string(math2::kind::plus), "plus");
    XCTAssertEqual(to_string(math2::kind::minus), "minus");
    XCTAssertEqual(to_string(math2::kind::multiply), "multiply");
    XCTAssertEqual(to_string(math2::kind::divide), "divide");

    XCTAssertEqual(to_string(math2::kind::atan2), "atan2");

    XCTAssertEqual(to_string(math2::kind::pow), "pow");
    XCTAssertEqual(to_string(math2::kind::hypot), "hypot");
}

- (void)test_input_to_string {
    XCTAssertEqual(to_string(math2::input::left), "left");
    XCTAssertEqual(to_string(math2::input::right), "right");
}

- (void)test_output_to_string {
    XCTAssertEqual(to_string(math2::output::result), "result");
}

@end
