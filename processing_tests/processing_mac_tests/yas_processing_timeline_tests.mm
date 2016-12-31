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

- (void)test_track {
    timeline timeline;

    track track1;
    timeline.insert_track(1, track1);

    track track2;
    timeline.insert_track(2, track2);

    track trackMinus1;
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

- (void)test_process {
    timeline timeline;

    time_range called_send_time_range;
    time_range called_receive_time_range;
    
    auto process_data = make_data<int16_t>(2);
    
    auto clear = [&called_send_time_range, &called_receive_time_range, &process_data](){
        called_send_time_range = {};
        called_receive_time_range = {};
        auto &data_raw = get_raw<int16_t>(process_data);
        for (auto const &idx : make_each(2)) {
            data_raw[idx] = 0;
        }
    };

    // setup track1 > インデックスをそのままセット

    track track1;
    timeline.insert_track(1, track1);

    auto send_handler1 = [](processing::time_range const &time_range, channel_index_t const ch_idx,
                            std::string const &key, int16_t *const signal_ptr) {
        if (key == "out") {
            for (auto const &idx : make_each(time_range.length)) {
                signal_ptr[idx] = idx;
            }
        }
    };

    auto processor1 = make_send_signal_processor<int16_t>({std::move(send_handler1)});
    auto module1 = module{{std::move(processor1)}};
    module1.connect_output("out", 0);

    track1.insert_module({.start_frame = 0, .length = 2}, module1);

    // setup track2 > +1する

    track track2;
    timeline.insert_track(2, track2);

    auto send_handler2 = [&process_data, &called_send_time_range](processing::time_range const &time_range,
                                                                  channel_index_t const ch_idx, std::string const &key,
                                                                  int16_t *const signal_ptr) {
        called_send_time_range = time_range;

        if (key == "out") {
            auto &data_raw = get_raw<int16_t>(process_data);
            for (auto const &idx : make_each(time_range.length)) {
                signal_ptr[idx] = data_raw[idx];
            }
        }
    };

    auto receive_handler2 = [&process_data, &called_receive_time_range](
        processing::time_range const &time_range, channel_index_t const ch_idx, std::string const &key,
        int16_t const *const signal_ptr) {
        called_receive_time_range = time_range;

        if (key == "in") {
            auto &data_raw = get_raw<int16_t>(process_data);
            for (auto const &idx : make_each(time_range.length)) {
                data_raw[idx] = signal_ptr[idx] + 1;
            }
        }
    };

    auto receive_processor2 = make_receive_signal_processor<int16_t>({std::move(receive_handler2)});
    auto send_processor2 = make_send_signal_processor<int16_t>({std::move(send_handler2)});
    auto module2 = module{{std::move(receive_processor2), std::move(send_processor2)}};

    module2.connect_input("in", 0);
    module2.connect_output("out", 0);

    track2.insert_module({.start_frame = 0, .length = 2}, module2);

    {
        stream stream{{.start_frame = 0, .length = 2}};
        
        timeline.process(stream);
        
        XCTAssertTrue((called_send_time_range == time_range{.start_frame = 0, .length = 2}));
        XCTAssertTrue((called_receive_time_range == time_range{.start_frame = 0, .length = 2}));
        
        XCTAssertTrue(stream.has_channel(0));
        auto &datas = stream.channel(0).datas();
        XCTAssertEqual(datas.size(), 1);
        auto const &data_raw = get_raw<int16_t>((*datas.begin()).second);
        XCTAssertEqual(data_raw.size(), 2);
        XCTAssertEqual(data_raw[0], 1);
        XCTAssertEqual(data_raw[1], 2);
    }
    
    clear();
    
    {
        stream stream{{.start_frame = -1, .length = 2}};
        
        timeline.process(stream);
        
        XCTAssertTrue((called_send_time_range == time_range{.start_frame = 0, .length = 1}));
        XCTAssertTrue((called_receive_time_range == time_range{.start_frame = 0, .length = 1}));
        
        XCTAssertTrue(stream.has_channel(0));
        auto &datas = stream.channel(0).datas();
        XCTAssertEqual(datas.size(), 1);
        auto const &data_raw = get_raw<int16_t>((*datas.begin()).second);
        XCTAssertEqual(data_raw.size(), 1);
        XCTAssertEqual(data_raw[0], 1);
    }
    
    clear();
    
    {
        stream stream{{.start_frame = 1, .length = 2}};
        
        timeline.process(stream);
        
        XCTAssertTrue((called_send_time_range == time_range{.start_frame = 1, .length = 1}));
        XCTAssertTrue((called_receive_time_range == time_range{.start_frame = 1, .length = 1}));
        
        XCTAssertTrue(stream.has_channel(0));
        auto &datas = stream.channel(0).datas();
        XCTAssertEqual(datas.size(), 1);
        auto const &data_raw = get_raw<int16_t>((*datas.begin()).second);
        XCTAssertEqual(data_raw.size(), 1);
        XCTAssertEqual(data_raw[0], 1);
    }
    
    clear();
    
    {
        stream stream{{.start_frame = 3, .length = 2}};
        
        timeline.process(stream);
        
        XCTAssertFalse(stream.has_channel(0));
    }
}

@end
