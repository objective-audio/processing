//
//  yas_processing_sub_timeline_module_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_sub_timeline_module_tests : XCTestCase

@end

@implementation yas_processing_sub_timeline_module_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_make_module {
    XCTAssertTrue(make_module(timeline{}));
}

- (void)test_process_signal {
    timeline main_timeline;
    timeline sub_timeline;

    {
        auto plus_module = make_signal_module<int8_t>(math2::kind::plus);
        plus_module.connect_input(to_connector_index(math2::input::left), 10);
        plus_module.connect_input(to_connector_index(math2::input::right), 11);
        plus_module.connect_output(to_connector_index(math2::output::result), 12);

        auto &sub_track = sub_timeline.add_track(0);
        sub_track.insert_module(time::range{1, 2}, std::move(plus_module));
    }

    {
        auto left_module = constant::make_signal_module(int8_t(7));
        left_module.connect_output(to_connector_index(constant::output::value), 0);

        auto right_module = constant::make_signal_module(int8_t(8));
        right_module.connect_output(to_connector_index(constant::output::value), 1);

        auto &main_track_0 = main_timeline.add_track(0);
        main_track_0.insert_module(time::range{0, 4}, std::move(left_module));

        auto &main_track_1 = main_timeline.add_track(1);
        main_track_1.insert_module(time::range{0, 4}, std::move(right_module));
    }

    {
        auto sub_timeline_module = make_module(std::move(sub_timeline));
        sub_timeline_module.connect_input(10, 0);
        sub_timeline_module.connect_input(11, 1);
        sub_timeline_module.connect_output(12, 20);

        auto &main_track_2 = main_timeline.add_track(2);
        main_track_2.insert_module(time::range{0, 4}, std::move(sub_timeline_module));
    }

    stream stream{sync_source{1, 4}};

    main_timeline.process(time::range{0, 4}, stream);

    XCTAssertEqual(stream.channel_count(), 3);
    XCTAssertTrue(stream.has_channel(0));
    XCTAssertTrue(stream.has_channel(1));
    XCTAssertTrue(stream.has_channel(20));

    {
        auto const &channel = stream.channel(0);
        XCTAssertEqual(channel.events().size(), 1);
        XCTAssertEqual(channel.events().begin()->first, make_range_time(0, 4));

        auto const signal = cast<signal_event>(channel.events().begin()->second);
        auto const &vec = signal.vector<int8_t>();
        XCTAssertEqual(vec.size(), 4);
        for (auto const &value : vec) {
            XCTAssertEqual(value, 7);
        }
    }

    {
        auto const &channel = stream.channel(1);
        XCTAssertEqual(channel.events().size(), 1);
        XCTAssertEqual(channel.events().begin()->first, make_range_time(0, 4));

        auto const signal = cast<signal_event>(channel.events().begin()->second);
        auto const &vec = signal.vector<int8_t>();
        XCTAssertEqual(vec.size(), 4);
        for (auto const &value : vec) {
            XCTAssertEqual(value, 8);
        }
    }

    {
        auto const &channel = stream.channel(20);
        XCTAssertEqual(channel.events().size(), 1);
        XCTAssertEqual(channel.events().begin()->first, make_range_time(1, 2));

        auto const signal = cast<signal_event>(channel.events().begin()->second);
        auto const &vec = signal.vector<int8_t>();
        XCTAssertEqual(vec.size(), 2);
        for (auto const &value : vec) {
            XCTAssertEqual(value, 15);
        }
    }
}

- (void)test_process_overwrite_signal {
    timeline main_timeline;
    timeline sub_timeline;

    {
        auto main_module = constant::make_signal_module(int8_t(1));
        main_module.connect_output(to_connector_index(constant::output::value), 0);

        auto &main_track = main_timeline.add_track(0);
        main_track.insert_module(time::range{0, 4}, std::move(main_module));
    }

    {
        auto sub_module = constant::make_signal_module(int8_t(2));
        sub_module.connect_output(to_connector_index(constant::output::value), 0);

        auto &sub_track = sub_timeline.add_track(0);
        sub_track.insert_module(time::range{1, 2}, std::move(sub_module));
    }

    {
        auto sub_timeline_module = make_module(std::move(sub_timeline));
        sub_timeline_module.connect_output(0, 0);

        auto &main_track_2 = main_timeline.add_track(1);
        main_track_2.insert_module(time::range{1, 2}, std::move(sub_timeline_module));
    }

    stream stream{sync_source{1, 4}};

    main_timeline.process(time::range{0, 4}, stream);

    XCTAssertEqual(stream.channel_count(), 1);
    XCTAssertTrue(stream.has_channel(0));

    {
        auto const &channel = stream.channel(0);
        XCTAssertEqual(channel.events().size(), 1);
        XCTAssertEqual(channel.events().begin()->first, make_range_time(0, 4));

        auto const signal = cast<signal_event>(channel.events().begin()->second);
        auto const &vec = signal.vector<int8_t>();
        XCTAssertEqual(vec.size(), 4);
        XCTAssertEqual(vec[0], 1);
        XCTAssertEqual(vec[1], 2);
        XCTAssertEqual(vec[2], 2);
        XCTAssertEqual(vec[3], 1);
    }
}

- (void)test_process_signal_offset {
    timeline main_timeline;
    timeline sub_timeline;

    {
        auto plus_module = make_signal_module<int8_t>(math2::kind::plus);
        plus_module.connect_input(to_connector_index(math2::input::left), 10);
        plus_module.connect_input(to_connector_index(math2::input::right), 11);
        plus_module.connect_output(to_connector_index(math2::output::result), 12);

        auto &sub_track = sub_timeline.add_track(0);
        sub_track.insert_module(time::range{0, 2}, std::move(plus_module));
    }

    {
        auto left_module = constant::make_signal_module(int8_t(7));
        left_module.connect_output(to_connector_index(constant::output::value), 0);

        auto right_module = constant::make_signal_module(int8_t(8));
        right_module.connect_output(to_connector_index(constant::output::value), 1);

        auto &main_track_0 = main_timeline.add_track(0);
        main_track_0.insert_module(time::range{0, 4}, std::move(left_module));

        auto &main_track_1 = main_timeline.add_track(1);
        main_track_1.insert_module(time::range{0, 4}, std::move(right_module));
    }

    {
        auto sub_timeline_module = make_module(std::move(sub_timeline), 1);
        sub_timeline_module.connect_input(10, 0);
        sub_timeline_module.connect_input(11, 1);
        sub_timeline_module.connect_output(12, 20);

        auto &main_track_2 = main_timeline.add_track(2);
        main_track_2.insert_module(time::range{1, 2}, std::move(sub_timeline_module));
    }

    stream stream{sync_source{1, 4}};

    main_timeline.process(time::range{0, 4}, stream);

    XCTAssertEqual(stream.channel_count(), 3);
    XCTAssertTrue(stream.has_channel(0));
    XCTAssertTrue(stream.has_channel(1));
    XCTAssertTrue(stream.has_channel(20));

    {
        auto const &channel = stream.channel(0);
        XCTAssertEqual(channel.events().size(), 1);
        XCTAssertEqual(channel.events().begin()->first, make_range_time(0, 4));

        auto const signal = cast<signal_event>(channel.events().begin()->second);
        auto const &vec = signal.vector<int8_t>();
        XCTAssertEqual(vec.size(), 4);
        for (auto const &value : vec) {
            XCTAssertEqual(value, 7);
        }
    }

    {
        auto const &channel = stream.channel(1);
        XCTAssertEqual(channel.events().size(), 1);
        XCTAssertEqual(channel.events().begin()->first, make_range_time(0, 4));

        auto const signal = cast<signal_event>(channel.events().begin()->second);
        auto const &vec = signal.vector<int8_t>();
        XCTAssertEqual(vec.size(), 4);
        for (auto const &value : vec) {
            XCTAssertEqual(value, 8);
        }
    }

    {
        auto const &channel = stream.channel(20);
        XCTAssertEqual(channel.events().size(), 1);
        XCTAssertEqual(channel.events().begin()->first, make_range_time(1, 2));

        auto const signal = cast<signal_event>(channel.events().begin()->second);
        auto const &vec = signal.vector<int8_t>();
        XCTAssertEqual(vec.size(), 2);
        for (auto const &value : vec) {
            XCTAssertEqual(value, 15);
        }
    }
}

@end
