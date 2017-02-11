//
//  yas_processing_number_to_signal_module_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"
#import "yas_boolean.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_number_to_signal_module_tests : XCTestCase

@end

@implementation yas_processing_number_to_signal_module_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_process {
    channel_index_t const in_ch_idx = 2;
    channel_index_t const out_ch_idx = 5;
    length_t const process_length = 8;

    stream stream{sync_source{1, process_length}};

    {
        auto &channel = stream.add_channel(in_ch_idx);
        channel.insert_event(make_frame_time(1), make_number_event<int8_t>(1));
        channel.insert_event(make_frame_time(3), make_number_event<int8_t>(3));
        channel.insert_event(make_frame_time(10), make_number_event<int8_t>(10));
        channel.insert_event(make_frame_time(13), make_number_event<int8_t>(13));
    }

    auto num_to_sig_module = make_number_to_signal_module<int8_t>();
    num_to_sig_module.connect_input(to_connector_index(number_to_signal::input::number), in_ch_idx);
    num_to_sig_module.connect_output(to_connector_index(number_to_signal::output::signal), out_ch_idx);

    num_to_sig_module.process({0, process_length}, stream);

    {
        auto const &channel = stream.channel(out_ch_idx);
        auto const &signal_events = channel.filtered_events<int8_t, signal_event>();

        XCTAssertEqual(signal_events.size(), 1);
        XCTAssertEqual(signal_events.begin()->first, (time::range{0, process_length}));

        auto const &signal_event = signal_events.begin()->second;

        auto const *const data = signal_event.data<int8_t>();

        XCTAssertEqual(data[0], 0);
        XCTAssertEqual(data[1], 1);
        XCTAssertEqual(data[2], 1);
        XCTAssertEqual(data[3], 3);
        XCTAssertEqual(data[4], 3);
        XCTAssertEqual(data[5], 3);
        XCTAssertEqual(data[6], 3);
        XCTAssertEqual(data[7], 3);
    }

    {
        auto const &channel = stream.channel(in_ch_idx);

        XCTAssertEqual(channel.events().size(), 2);

        auto iterator = channel.events().begin();
        XCTAssertEqual(iterator->first, make_frame_time(10));
        ++iterator;
        XCTAssertEqual(iterator->first, make_frame_time(13));
    }

    stream.channel(out_ch_idx).erase_event_if([](auto const &) { return true; });

    XCTAssertEqual(stream.channel(out_ch_idx).events().size(), 0);

    num_to_sig_module.process({process_length, process_length}, stream);

    {
        auto const &channel = stream.channel(out_ch_idx);
        auto const &signal_events = channel.filtered_events<int8_t, signal_event>();

        XCTAssertEqual(signal_events.size(), 1);
        XCTAssertEqual(signal_events.begin()->first, (time::range{process_length, process_length}));

        auto const &signal_event = signal_events.begin()->second;
        auto const *const data = signal_event.data<int8_t>();

        XCTAssertEqual(data[0], 3);
        XCTAssertEqual(data[1], 3);
        XCTAssertEqual(data[2], 10);
        XCTAssertEqual(data[3], 10);
        XCTAssertEqual(data[4], 10);
        XCTAssertEqual(data[5], 13);
        XCTAssertEqual(data[6], 13);
        XCTAssertEqual(data[7], 13);
    }
}

@end
