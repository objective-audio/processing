//
//  yas_processing_timeline_tests.mm
//

#import <XCTest/XCTest.h>
#import <cpp_utils/yas_each_index.h>
#import <processing/yas_processing_umbrella.h>

using namespace yas;
using namespace yas::proc;

@interface yas_processing_timeline_tests : XCTestCase

@end

@implementation yas_processing_timeline_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create {
    timeline timeline;

    XCTAssertTrue(timeline);
    XCTAssertEqual(timeline.track_count(), 0);
}

- (void)test_create_null {
    timeline timeline = nullptr;

    XCTAssertFalse(timeline);
}

- (void)test_insert_track {
    timeline timeline;

    track track1;
    track track2;
    track trackMinus1;

    timeline.insert_track(1, track1);
    timeline.insert_track(2, track2);
    timeline.insert_track(-1, trackMinus1);

    XCTAssertEqual(timeline.track_count(), 3);

    XCTAssertTrue(timeline.has_track(1));
    XCTAssertTrue(timeline.has_track(2));
    XCTAssertTrue(timeline.has_track(-1));

    XCTAssertTrue(timeline.track(1) == track1);
    XCTAssertTrue(timeline.track(2) == track2);
    XCTAssertTrue(timeline.track(-1) == trackMinus1);

    XCTAssertFalse(timeline.has_track(0));
}

- (void)test_insert_track_result {
    timeline timeline;

    track track_a;

    XCTAssertTrue(timeline.insert_track(0, track_a));

    track track_b;

    XCTAssertFalse(timeline.insert_track(0, track_b));

    XCTAssertEqual(timeline.track(0), track_a);
}

- (void)test_erase_track {
    timeline timeline;

    timeline.insert_track(10, proc::track{});
    timeline.insert_track(11, proc::track{});

    timeline.erase_track(10);

    XCTAssertEqual(timeline.track_count(), 1);
    XCTAssertFalse(timeline.has_track(10));
    XCTAssertTrue(timeline.has_track(11));
}

- (void)test_process {
    timeline timeline;

    proc::time called_send_time = nullptr;
    proc::time called_receive_time = nullptr;

    auto process_signal = make_signal_event<int16_t>(2);

    auto clear = [&called_send_time, &called_receive_time, &process_signal]() {
        called_send_time = {};
        called_receive_time = {};
        auto &vec = process_signal.vector<int16_t>();
        for (auto const &idx : make_each_index(2)) {
            vec[idx] = 0;
        }
    };

    // setup track1 > インデックスをそのままセット

    track track1;

    auto module1 = module{[] {
        auto send_handler1 = [](proc::time::range const &time_range, sync_source const &, channel_index_t const ch_idx,
                                connector_index_t const co_idx, int16_t *const signal_ptr) {
            if (co_idx == 0) {
                for (auto const &idx : make_each_index(time_range.length)) {
                    signal_ptr[idx] = idx;
                }
            }
        };
        return module::processors_t{make_send_signal_processor<int16_t>({std::move(send_handler1)})};
    }};
    module1.connect_output(0, 0);

    track1.push_back_module({0, 2}, module1);
    timeline.insert_track(1, track1);

    // setup track2 > +1する

    track track2;

    auto send_handler2 = [&process_signal, &called_send_time](
                             proc::time::range const &time_range, sync_source const &, channel_index_t const ch_idx,
                             connector_index_t const co_idx, int16_t *const signal_ptr) {
        called_send_time = proc::time{time_range};

        if (co_idx == 0) {
            auto &vec = process_signal.vector<int16_t>();
            for (auto const &idx : make_each_index(time_range.length)) {
                signal_ptr[idx] = vec[idx];
            }
        }
    };

    auto receive_handler2 = [&process_signal, &called_receive_time](
                                proc::time::range const &time_range, sync_source const &, channel_index_t const ch_idx,
                                connector_index_t const co_idx, int16_t const *const signal_ptr) {
        called_receive_time = proc::time{time_range};

        if (co_idx == 0) {
            auto &vec = process_signal.vector<int16_t>();
            for (auto const &idx : make_each_index(time_range.length)) {
                vec[idx] = signal_ptr[idx] + 1;
            }
        }
    };

    auto module2 = module{[receive_handler2 = std::move(receive_handler2), send_handler2 = std::move(send_handler2)] {
        auto receive_processor2 = make_receive_signal_processor<int16_t>({std::move(receive_handler2)});
        auto send_processor2 = make_send_signal_processor<int16_t>({std::move(send_handler2)});
        return module::processors_t{{std::move(receive_processor2), std::move(send_processor2)}};
    }};

    module2.connect_input(0, 0);
    module2.connect_output(0, 0);

    track2.push_back_module({0, 2}, module2);
    timeline.insert_track(2, track2);

    {
        stream stream{sync_source{1, 2}};

        timeline.process({0, 2}, stream);

        XCTAssertTrue((called_send_time == proc::time{0, 2}));
        XCTAssertTrue((called_receive_time == proc::time{0, 2}));

        XCTAssertTrue(stream.has_channel(0));
        auto &events = stream.channel(0).events();
        XCTAssertEqual(events.size(), 1);
        auto const &vec = cast<proc::signal_event>(events.cbegin()->second).vector<int16_t>();
        XCTAssertEqual(vec.size(), 2);
        XCTAssertEqual(vec[0], 1);
        XCTAssertEqual(vec[1], 2);
    }

    {
        clear();

        stream stream{sync_source{1, 2}};

        timeline.process({-1, 2}, stream);

        XCTAssertTrue((called_send_time == proc::time{0, 1}));
        XCTAssertTrue((called_receive_time == proc::time{0, 1}));

        XCTAssertTrue(stream.has_channel(0));
        auto &events = stream.channel(0).events();
        XCTAssertEqual(events.size(), 1);
        auto const &vec = cast<proc::signal_event>(events.cbegin()->second).vector<int16_t>();
        XCTAssertEqual(vec.size(), 1);
        XCTAssertEqual(vec[0], 1);
    }

    {
        clear();

        stream stream{sync_source{1, 2}};

        timeline.process({1, 2}, stream);

        XCTAssertTrue((called_send_time == proc::time{1, 1}));
        XCTAssertTrue((called_receive_time == proc::time{1, 1}));

        XCTAssertTrue(stream.has_channel(0));
        auto &events = stream.channel(0).events();
        XCTAssertEqual(events.size(), 1);
        auto const &vec = cast<proc::signal_event>(events.cbegin()->second).vector<int16_t>();
        XCTAssertEqual(vec.size(), 1);
        XCTAssertEqual(vec[0], 1);
    }

    {
        clear();

        stream stream{sync_source{1, 2}};

        timeline.process({3, 2}, stream);

        XCTAssertFalse(stream.has_channel(0));
    }
}

- (void)test_process_offline {
    timeline timeline;

    channel_index_t const ch_idx = 0;
    length_t const process_length = 5;

    proc::track track;
    auto fast_each = make_fast_each<frame_index_t>(process_length);
    while (yas_each_next(fast_each)) {
        auto const &idx = yas_each_index(fast_each);
        auto module = make_signal_module<int8_t>(idx);
        module.connect_output(to_connector_index(constant::output::value), ch_idx);
        track.push_back_module({idx, 1}, std::move(module));
    }
    timeline.insert_track(0, track);

    std::vector<std::pair<time::range, std::vector<int8_t>>> called;

    timeline.process(time::range{0, process_length}, sync_source{1, 2},
                     [&ch_idx, &called](time::range const &time_range, stream const &stream, bool &) {
                         auto const &channel = stream.channel(ch_idx);
                         auto const &pair = *channel.events().cbegin();
                         auto const signal = cast<signal_event>(pair.second);
                         called.emplace_back(std::make_pair(pair.first.get<time::range>(), signal.vector<int8_t>()));
                     });

    XCTAssertEqual(called.size(), 3);

    XCTAssertEqual(called[0].first, (time::range{0, 2}));
    XCTAssertEqual(called[0].second[0], 0);
    XCTAssertEqual(called[0].second[1], 1);

    XCTAssertEqual(called[1].first, (time::range{2, 2}));
    XCTAssertEqual(called[1].second[0], 2);
    XCTAssertEqual(called[1].second[1], 3);

    XCTAssertEqual(called[2].first, (time::range{4, 1}));
    XCTAssertEqual(called[2].second[0], 4);
}

- (void)test_stop_process_offline {
    timeline timeline;

    length_t const process_length = 10;
    frame_index_t last_frame = 0;

    timeline.process(time::range{0, process_length}, sync_source{1, 1},
                     [&last_frame](time::range const &time_range, stream const &, bool &stop) {
                         last_frame = time_range.frame;

                         if (time_range.frame == 5) {
                             stop = true;
                         }
                     });

    XCTAssertEqual(last_frame, 5);
}

- (void)test_total_range {
    timeline timeline;

    XCTAssertFalse(timeline.total_range());

    proc::track track_0;
    track_0.push_back_module({0, 1}, proc::module{[] { return module::processors_t{}; }});
    timeline.insert_track(0, track_0);

    XCTAssertEqual(timeline.total_range(), (proc::time::range{0, 1}));

    proc::track track_1;
    track_1.push_back_module({1, 1}, proc::module{[] { return module::processors_t{}; }});
    timeline.insert_track(1, track_1);

    XCTAssertEqual(timeline.total_range(), (proc::time::range{0, 2}));

    proc::track track_2;
    track_2.push_back_module({99, 1}, proc::module{[] { return module::processors_t{}; }});
    timeline.insert_track(2, track_2);

    XCTAssertEqual(timeline.total_range(), (proc::time::range{0, 100}));

    proc::track track_3;
    track_3.push_back_module({-10, 1}, proc::module{[] { return module::processors_t{}; }});
    timeline.insert_track(3, track_3);

    XCTAssertEqual(timeline.total_range(), (proc::time::range{-10, 110}));
}

- (void)test_copy {
    std::vector<int> called;

    auto index = std::make_shared<int>(0);
    proc::module module{[index = std::move(index), &called] {
        auto processor = [index = *index, &called](time::range const &, connector_map_t const &,
                                                   connector_map_t const &, stream &) { called.push_back(index); };
        ++(*index);
        return module::processors_t{std::move(processor)};
    }};

    proc::track track;
    track.push_back_module({0, 1}, std::move(module));

    proc::timeline timeline;
    timeline.insert_track(0, std::move(track));

    auto copied_timeline = timeline.copy();

    XCTAssertEqual(copied_timeline.tracks().size(), 1);
    XCTAssertTrue(copied_timeline.has_track(0));
    XCTAssertEqual(copied_timeline.track(0).modules().size(), 1);
    XCTAssertEqual(copied_timeline.track(0).modules().count({0, 1}), 1);

    proc::stream stream{sync_source{1, 1}};

    timeline.process({0, 1}, stream);

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), 0);

    copied_timeline.process({0, 1}, stream);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), 1);
}

@end
