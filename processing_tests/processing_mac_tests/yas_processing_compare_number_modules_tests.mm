//
//  yas_processing_compare_number_modules_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing_test_utils.h"
#import "yas_boolean.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_compare_number_modules_tests : XCTestCase

@end

@implementation yas_processing_compare_number_modules_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_make_number_module {
    XCTAssertTrue(make_number_module<int16_t>(compare::kind::is_equal));
    XCTAssertTrue(make_number_module<int16_t>(compare::kind::is_not_equal));
    XCTAssertTrue(make_number_module<int16_t>(compare::kind::is_greater));
    XCTAssertTrue(make_number_module<int16_t>(compare::kind::is_greater_equal));
    XCTAssertTrue(make_number_module<int16_t>(compare::kind::is_less));
    XCTAssertTrue(make_number_module<int16_t>(compare::kind::is_less_equal));
}

- (void)test_is_equal {
    length_t const process_length = 6;

    int16_t const left_data[3] = {
        1, 2, 3,
    };

    int16_t const right_data[3] = {
        2, 2, 2,
    };

    auto stream = test::make_number_stream(left_data, time::range{1, 3}, 0, right_data, time::range{2, 3}, 1);

    auto module = make_number_module<int16_t>(compare::kind::is_equal);
    module.connect_input(to_connector_index(compare::input::left), 0);
    module.connect_input(to_connector_index(compare::input::right), 1);
    module.connect_output(to_connector_index(compare::output::result), 2);

    module.process({0, process_length}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const events = stream.channel(2).filtered_events<boolean, number_event>();

    XCTAssertEqual(events.size(), 4);

    auto event_iterator = events.begin();

    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 1 == 0
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 2 == 2
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 3 == 2
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 0 == 2
}

- (void)test_is_not_equal {
    length_t const process_length = 6;

    int16_t const left_data[3] = {
        1, 2, 3,
    };

    int16_t const right_data[3] = {
        2, 2, 2,
    };

    auto stream = test::make_number_stream(left_data, time::range{1, 3}, 0, right_data, time::range{2, 3}, 1);

    auto calc_module = make_number_module<int16_t>(compare::kind::is_not_equal);
    calc_module.connect_input(to_connector_index(compare::input::left), 0);
    calc_module.connect_input(to_connector_index(compare::input::right), 1);
    calc_module.connect_output(to_connector_index(compare::output::result), 2);

    calc_module.process({0, process_length}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const events = stream.channel(2).filtered_events<boolean, number_event>();

    XCTAssertEqual(events.size(), 4);

    auto event_iterator = events.begin();

    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 1 != 0
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 2 != 2
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 3 != 2
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 0 != 2
}

- (void)test_is_greater {
    length_t const process_length = 11;

    int16_t const left_data[7] = {
        -1, 0, 1, 2, 3, 4, 0,
    };

    int16_t const right_data[6] = {
        3, 3, 3, 1, 0, -1,
    };

    auto stream = test::make_number_stream(left_data, time::range{1, 7}, 0, right_data, time::range{4, 6}, 1);

    auto calc_module = make_number_module<int16_t>(compare::kind::is_greater);
    calc_module.connect_input(to_connector_index(compare::input::left), 0);
    calc_module.connect_input(to_connector_index(compare::input::right), 1);
    calc_module.connect_output(to_connector_index(compare::output::result), 2);

    calc_module.process({0, process_length}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const events = stream.channel(2).filtered_events<boolean, number_event>();

    XCTAssertEqual(events.size(), 9);

    auto event_iterator = events.begin();

    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // -1 > 0
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 0 > 0
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 1 > 0
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 2 > 3
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 3 > 3
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 4 > 3
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 0 > 1
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 0 > 0
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 0 > -1
}

- (void)test_is_greater_equal {
    length_t const process_length = 11;

    int16_t const left_data[7] = {
        -1, 0, 1, 2, 3, 4, 0,
    };

    int16_t const right_data[6] = {
        3, 3, 3, 1, 0, -1,
    };

    auto stream = test::make_number_stream(left_data, time::range{1, 7}, 0, right_data, time::range{4, 6}, 1);

    auto calc_module = make_number_module<int16_t>(compare::kind::is_greater_equal);
    calc_module.connect_input(to_connector_index(compare::input::left), 0);
    calc_module.connect_input(to_connector_index(compare::input::right), 1);
    calc_module.connect_output(to_connector_index(compare::output::result), 2);

    calc_module.process({0, process_length}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const events = stream.channel(2).filtered_events<boolean, number_event>();

    XCTAssertEqual(events.size(), 9);

    auto event_iterator = events.begin();

    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // -1 >= 0
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 0 >= 0
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 1 >= 0
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 2 >= 3
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 3 >= 3
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 4 >= 3
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 0 >= 1
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 0 >= 0
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 0 >= -1
}

- (void)test_is_less {
    length_t const process_length = 11;

    int16_t const left_data[7] = {
        -1, 0, 1, 2, 3, 4, 0,
    };

    int16_t const right_data[6] = {
        3, 3, 3, 1, 0, -1,
    };

    auto stream = test::make_number_stream(left_data, time::range{1, 7}, 0, right_data, time::range{4, 6}, 1);

    auto calc_module = make_number_module<int16_t>(compare::kind::is_less);
    calc_module.connect_input(to_connector_index(compare::input::left), 0);
    calc_module.connect_input(to_connector_index(compare::input::right), 1);
    calc_module.connect_output(to_connector_index(compare::output::result), 2);

    calc_module.process({0, process_length}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const events = stream.channel(2).filtered_events<boolean, number_event>();

    XCTAssertEqual(events.size(), 9);

    auto event_iterator = events.begin();

    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // -1 < 0
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 0 < 0
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 1 < 0
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 2 < 3
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 3 < 3
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 4 < 3
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 0 < 1
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 0 < 0
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 0 < -1
}

- (void)test_is_less_equal {
    length_t const process_length = 11;

    int16_t const left_data[7] = {
        -1, 0, 1, 2, 3, 4, 0,
    };

    int16_t const right_data[6] = {
        3, 3, 3, 1, 0, -1,
    };

    auto stream = test::make_number_stream(left_data, time::range{1, 7}, 0, right_data, time::range{4, 6}, 1);

    auto calc_module = make_number_module<int16_t>(compare::kind::is_less_equal);
    calc_module.connect_input(to_connector_index(compare::input::left), 0);
    calc_module.connect_input(to_connector_index(compare::input::right), 1);
    calc_module.connect_output(to_connector_index(compare::output::result), 2);

    calc_module.process({0, process_length}, stream);

    XCTAssertTrue(stream.has_channel(2));

    auto const events = stream.channel(2).filtered_events<boolean, number_event>();

    XCTAssertEqual(events.size(), 9);

    auto event_iterator = events.begin();

    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // -1 <= 0
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 0 <= 0
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 1 <= 0
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 2 <= 3
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 3 <= 3
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 4 <= 3
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 0 <= 1
    XCTAssertTrue((event_iterator++)->second.get<boolean>());   // 0 <= 0
    XCTAssertFalse((event_iterator++)->second.get<boolean>());  // 0 <= -1
}

@end
