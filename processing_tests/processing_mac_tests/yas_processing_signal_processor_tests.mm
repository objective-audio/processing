//
//  yas_processing_signal_processor_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"
#import "yas_each_index.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_signal_processor_tests : XCTestCase

@end

@implementation yas_processing_signal_processor_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_send_signal_processor {
    auto const ch_idx = 5;
    connector_index_t const out_con_idx = 2;

    processing::time called_time = nullptr;
    connector_index_t called_con_idx;
    channel_index_t called_ch_idx;

    auto clear = [&called_time, &called_con_idx, &called_ch_idx]() {
        called_time = make_any_time();
        called_time = make_any_time();
        called_con_idx = 0;
        called_ch_idx = 0;
    };

    auto handler = [&called_time, &called_con_idx, &called_ch_idx](
        processing::time::range const &time_range, channel_index_t const ch_idx, connector_index_t const con_idx,
        int64_t *const signal_ptr) {
        called_time = processing::time{time_range};
        called_con_idx = con_idx;
        called_ch_idx = ch_idx;
        for (auto const &idx : make_each(time_range.length)) {
            signal_ptr[idx] = idx + time_range.frame;
        }
    };

    auto processor = processing::make_send_signal_processor<int64_t>(std::move(handler));

    auto module = processing::module{{std::move(processor)}};
    module.connect_output(out_con_idx, ch_idx);

    {
        clear();

        processing::stream stream;

        module.process({0, 2}, stream);

        XCTAssertEqual(called_time.get<time::range>().frame, 0);
        XCTAssertEqual(called_time.get<time::range>().length, 2);
        XCTAssertEqual(called_con_idx, out_con_idx);
        XCTAssertEqual(called_ch_idx, ch_idx);

        XCTAssertTrue(stream.has_channel(ch_idx));
        auto const &vec =
            cast<processing::signal_event>((*stream.channel(ch_idx).events().begin()).second).vector<int64_t>();
        XCTAssertEqual(vec.size(), 2);
        XCTAssertEqual(vec.at(0), 0);
        XCTAssertEqual(vec.at(1), 1);
    }

    {
        clear();

        processing::stream stream;

        module.process({1, 1}, stream);

        XCTAssertEqual(called_time.get<time::range>().frame, 1);
        XCTAssertEqual(called_time.get<time::range>().length, 1);
        XCTAssertEqual(called_con_idx, out_con_idx);
        XCTAssertEqual(called_ch_idx, ch_idx);

        XCTAssertTrue(stream.has_channel(ch_idx));
        auto const &vec =
            cast<processing::signal_event>((*stream.channel(ch_idx).events().begin()).second).vector<int64_t>();
        XCTAssertEqual(vec.size(), 1);
        XCTAssertEqual(vec.at(0), 1);
    }

    {
        clear();

        processing::stream stream;

        module.process({0, 1}, stream);

        XCTAssertEqual(called_time.get<time::range>().frame, 0);
        XCTAssertEqual(called_time.get<time::range>().length, 1);
        XCTAssertEqual(called_con_idx, out_con_idx);
        XCTAssertEqual(called_ch_idx, ch_idx);

        XCTAssertTrue(stream.has_channel(ch_idx));
        auto const &vec =
            cast<processing::signal_event>((*stream.channel(ch_idx).events().begin()).second).vector<int64_t>();
        XCTAssertEqual(vec.size(), 1);
        XCTAssertEqual(vec.at(0), 0);
    }
}

- (void)test_receive_signal_processor {
    auto const ch_idx = 7;
    connector_index_t const in_con_idx = 10;

    processing::time called_time = nullptr;
    connector_index_t called_con_idx;
    channel_index_t called_ch_idx;
    int64_t called_signal[2];

    auto stream_signal = processing::make_signal_event<int64_t>(2);
    auto &stream_vec = stream_signal.vector<int64_t>();
    stream_vec[0] = 10;
    stream_vec[1] = 11;

    auto clear = [&called_time, &called_con_idx, &called_ch_idx, &called_signal]() {
        called_time = make_any_time();
        called_con_idx = 0;
        called_ch_idx = 0;
        called_signal[0] = 0.0;
        called_signal[1] = 0.0;
    };

    auto make_stream = [&stream_signal, &ch_idx](time::range const &time_range) {
        processing::stream stream;
        stream.add_channel(ch_idx);

        auto &channel = stream.channel(ch_idx);
        channel.insert_event(processing::time{time_range}, stream_signal);

        return stream;
    };

    auto handler = [&called_time, &called_con_idx, &called_ch_idx, &called_signal](
        processing::time::range const &time_range, channel_index_t const ch_idx, connector_index_t const con_idx,
        int64_t const *const signal_ptr) {
        called_time = processing::time{time_range};
        called_con_idx = con_idx;
        called_ch_idx = ch_idx;
        for (auto const &idx : make_each(time_range.length)) {
            called_signal[idx] = signal_ptr[idx];
        }
    };

    auto processor = make_receive_signal_processor<int64_t>(std::move(handler));

    auto module = processing::module{{std::move(processor)}};
    module.connect_input(in_con_idx, ch_idx);

    {
        clear();

        auto stream = make_stream({0, 2});

        XCTAssertNoThrow(module.process({0, 2}, stream));

        XCTAssertEqual(called_time.get<time::range>().frame, 0);
        XCTAssertEqual(called_time.get<time::range>().length, 2);
        XCTAssertEqual(called_con_idx, in_con_idx);
        XCTAssertEqual(called_ch_idx, ch_idx);

        XCTAssertEqual(called_signal[0], 10);
        XCTAssertEqual(called_signal[1], 11);
    }

    {
        clear();

        auto stream = make_stream({0, 2});

        XCTAssertNoThrow(module.process({0, 1}, stream));

        XCTAssertEqual(called_time.get<time::range>().frame, 0);
        XCTAssertEqual(called_time.get<time::range>().length, 1);
        XCTAssertEqual(called_con_idx, in_con_idx);
        XCTAssertEqual(called_ch_idx, ch_idx);

        XCTAssertEqual(called_signal[0], 10);
        XCTAssertEqual(called_signal[1], 0);
    }

    {
        clear();

        auto stream = make_stream({0, 2});

        XCTAssertNoThrow(module.process({1, 1}, stream));

        XCTAssertEqual(called_time.get<time::range>().frame, 1);
        XCTAssertEqual(called_time.get<time::range>().length, 1);
        XCTAssertEqual(called_con_idx, in_con_idx);
        XCTAssertEqual(called_ch_idx, ch_idx);

        XCTAssertEqual(called_signal[0], 11);
        XCTAssertEqual(called_signal[1], 0);
    }
}

- (void)test_receive_and_send_signal_processor {
    auto const receive_ch_idx = 3;
    auto const send_ch_idx = 9;
    connector_index_t const out_con_idx = 5;
    connector_index_t const in_con_idx = 6;

    processing::time process_time{0, 2};

    processing::stream stream;
    stream.add_channel(receive_ch_idx);

    auto input_stream_signal = processing::make_signal_event<int16_t>(2);
    auto &input_stream_vec = input_stream_signal.vector<int16_t>();
    input_stream_vec[0] = 1;
    input_stream_vec[1] = 2;

    auto &channel = stream.channel(receive_ch_idx);
    channel.insert_event(process_time, input_stream_signal);

    auto process_signal = processing::make_signal_event<int16_t>(2);

    auto receive_handler = [&process_signal](processing::time::range const &time_range, channel_index_t const ch_idx,
                                             connector_index_t const, int16_t const *const signal_ptr) {
        auto &process_vec = process_signal.vector<int16_t>();
        for (auto const &idx : make_each(time_range.length)) {
            process_vec[idx] = signal_ptr[idx] * 2;
        }
    };

    auto send_handler = [&process_signal](processing::time::range const &time_range, channel_index_t const ch_idx,
                                          connector_index_t const, int16_t *const signal_ptr) {
        auto &process_vec = process_signal.vector<int16_t>();
        for (auto const &idx : make_each(time_range.length)) {
            signal_ptr[idx] = process_vec[idx];
        }
    };

    auto receive_processor = make_receive_signal_processor<int16_t>(std::move(receive_handler));
    auto send_processor = processing::make_send_signal_processor<int16_t>(std::move(send_handler));
    auto module = processing::module{{std::move(receive_processor), std::move(send_processor)}};
    module.connect_input(in_con_idx, receive_ch_idx);
    module.connect_output(out_con_idx, send_ch_idx);

    module.process(process_time.get<time::range>(), stream);

    XCTAssertTrue(stream.has_channel(send_ch_idx));

    auto const &send_channel = stream.channel(send_ch_idx);
    auto const &send_vec = cast<processing::signal_event>((*send_channel.events().begin()).second).vector<int16_t>();

    XCTAssertEqual(send_vec.size(), 2);
    XCTAssertEqual(send_vec[0], 2);
    XCTAssertEqual(send_vec[1], 4);

    XCTAssertTrue(stream.has_channel(receive_ch_idx));

    auto const &receive_channel = stream.channel(receive_ch_idx);
    auto const &receive_vec =
        cast<processing::signal_event>((*receive_channel.events().begin()).second).vector<int16_t>();

    XCTAssertEqual(receive_vec.size(), 2);
    XCTAssertEqual(receive_vec[0], 1);
    XCTAssertEqual(receive_vec[1], 2);
}

@end
