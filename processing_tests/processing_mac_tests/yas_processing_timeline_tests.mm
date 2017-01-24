//
//  yas_processing_timeline_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"
#import "yas_each_index.h"

using namespace yas;
using namespace yas::processing;

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

- (void)test_add_track {
    timeline timeline;

    track &track1 = timeline.add_track(1);
    track &track2 = timeline.add_track(2);
    track &trackMinus1 = timeline.add_track(-1);

    XCTAssertEqual(timeline.track_count(), 3);

    XCTAssertTrue(timeline.has_track(1));
    XCTAssertTrue(timeline.has_track(2));
    XCTAssertTrue(timeline.has_track(-1));

    XCTAssertTrue(timeline.track(1) == track1);
    XCTAssertTrue(timeline.track(2) == track2);
    XCTAssertTrue(timeline.track(-1) == trackMinus1);

    XCTAssertFalse(timeline.has_track(0));
}

- (void)test_remove_track {
    timeline timeline;
    
    timeline.add_track(10);
    
    XCTAssertTrue(timeline.has_track(10));
    
    timeline.remove_track(10);
    
    XCTAssertFalse(timeline.has_track(10));
}

- (void)test_process {
    timeline timeline;

    processing::time called_send_time = nullptr;
    processing::time called_receive_time = nullptr;

    auto process_signal = make_signal_event<int16_t>(2);

    auto clear = [&called_send_time, &called_receive_time, &process_signal]() {
        called_send_time = {};
        called_receive_time = {};
        auto &vec = process_signal.vector<int16_t>();
        for (auto const &idx : make_each(2)) {
            vec[idx] = 0;
        }
    };

    // setup track1 > インデックスをそのままセット

    track &track1 = timeline.add_track(1);

    auto send_handler1 = [](processing::time::range const &time_range, sync_source const &,
                            channel_index_t const ch_idx, connector_index_t const con_idx, int16_t *const signal_ptr) {
        if (con_idx == 0) {
            for (auto const &idx : make_each(time_range.length)) {
                signal_ptr[idx] = idx;
            }
        }
    };

    auto processor1 = make_send_signal_processor<int16_t>({std::move(send_handler1)});
    auto module1 = module{{std::move(processor1)}};
    module1.connect_output(0, 0);

    track1.insert_module({0, 2}, module1);

    // setup track2 > +1する

    track &track2 = timeline.add_track(2);

    auto send_handler2 = [&process_signal, &called_send_time](
        processing::time::range const &time_range, sync_source const &, channel_index_t const ch_idx,
        connector_index_t const con_idx, int16_t *const signal_ptr) {
        called_send_time = processing::time{time_range};

        if (con_idx == 0) {
            auto &vec = process_signal.vector<int16_t>();
            for (auto const &idx : make_each(time_range.length)) {
                signal_ptr[idx] = vec[idx];
            }
        }
    };

    auto receive_handler2 = [&process_signal, &called_receive_time](
        processing::time::range const &time_range, sync_source const &, channel_index_t const ch_idx,
        connector_index_t const con_idx, int16_t const *const signal_ptr) {
        called_receive_time = processing::time{time_range};

        if (con_idx == 0) {
            auto &vec = process_signal.vector<int16_t>();
            for (auto const &idx : make_each(time_range.length)) {
                vec[idx] = signal_ptr[idx] + 1;
            }
        }
    };

    auto receive_processor2 = make_receive_signal_processor<int16_t>({std::move(receive_handler2)});
    auto send_processor2 = make_send_signal_processor<int16_t>({std::move(send_handler2)});
    auto module2 = module{{std::move(receive_processor2), std::move(send_processor2)}};

    module2.connect_input(0, 0);
    module2.connect_output(0, 0);

    track2.insert_module({0, 2}, module2);

    {
        stream stream{sync_source{1, 2}};

        timeline.process({0, 2}, stream);

        XCTAssertTrue((called_send_time == processing::time{0, 2}));
        XCTAssertTrue((called_receive_time == processing::time{0, 2}));

        XCTAssertTrue(stream.has_channel(0));
        auto &events = stream.channel(0).events();
        XCTAssertEqual(events.size(), 1);
        auto const &vec = cast<processing::signal_event>((*events.begin()).second).vector<int16_t>();
        XCTAssertEqual(vec.size(), 2);
        XCTAssertEqual(vec[0], 1);
        XCTAssertEqual(vec[1], 2);
    }

    {
        clear();

        stream stream{sync_source{1, 2}};

        timeline.process({-1, 2}, stream);

        XCTAssertTrue((called_send_time == processing::time{0, 1}));
        XCTAssertTrue((called_receive_time == processing::time{0, 1}));

        XCTAssertTrue(stream.has_channel(0));
        auto &events = stream.channel(0).events();
        XCTAssertEqual(events.size(), 1);
        auto const &vec = cast<processing::signal_event>((*events.begin()).second).vector<int16_t>();
        XCTAssertEqual(vec.size(), 1);
        XCTAssertEqual(vec[0], 1);
    }

    {
        clear();

        stream stream{sync_source{1, 2}};

        timeline.process({1, 2}, stream);

        XCTAssertTrue((called_send_time == processing::time{1, 1}));
        XCTAssertTrue((called_receive_time == processing::time{1, 1}));

        XCTAssertTrue(stream.has_channel(0));
        auto &events = stream.channel(0).events();
        XCTAssertEqual(events.size(), 1);
        auto const &vec = cast<processing::signal_event>((*events.begin()).second).vector<int16_t>();
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

@end
