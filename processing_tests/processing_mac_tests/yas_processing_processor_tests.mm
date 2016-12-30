//
//  yas_processing_processor_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"
#import "yas_each_index.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_processor_tests : XCTestCase

@end

@implementation yas_processing_processor_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create_null {
    processing::processor processor = nullptr;

    XCTAssertFalse(processor);
}

- (void)test_process_send_signal {
    auto const ch_idx = 5;
    auto const output_connector_key = "output";

    time_range process_time_range{.length = 2};
    stream stream = nullptr;
    time_range called_time_range;
    std::string called_key;
    connector::channel_index_t called_ch_idx;

    auto clear = [&called_time_range, &called_key, &called_ch_idx]() {
        called_time_range.start_frame = 0;
        called_time_range.length = 0;
        called_key.clear();
        called_ch_idx = 0;
    };

    auto handler = [&called_time_range, &called_key, &called_ch_idx](processing::time_range const &time_range,
                                                                     connector::channel_index_t const ch_idx,
                                                                     std::string const &key, double *const signal_ptr) {
        called_time_range = time_range;
        called_key = key;
        called_ch_idx = ch_idx;
        for (auto const &idx : make_each(time_range.length)) {
            signal_ptr[idx] = 1.0;
        }
    };

    auto processor = processing::make_send_signal_processor<double>(std::move(handler));

    auto module =
        processing::module{{.time_range = {.start_frame = 1, .length = 4}, .processors = {std::move(processor)}}};
    module.connect_output(output_connector_key, ch_idx);

    {
        clear();

        process_time_range.start_frame = 0;
        stream = processing::stream{process_time_range};

        module.process(stream);

        XCTAssertEqual(called_time_range.start_frame, 1);
        XCTAssertEqual(called_time_range.length, 1);
        XCTAssertEqual(called_key, output_connector_key);
        XCTAssertEqual(called_ch_idx, ch_idx);

        XCTAssertTrue(stream.has_channel(ch_idx));
        auto const &raw = get_raw<double>((*stream.channel(ch_idx).datas().begin()).second);
        XCTAssertEqual(raw.size(), 1);
        XCTAssertEqual(raw.at(0), 1.0);
    }

    {
        clear();

        process_time_range.start_frame = 2;
        stream = processing::stream{process_time_range};

        module.process(stream);

        XCTAssertEqual(called_time_range.start_frame, 2);
        XCTAssertEqual(called_time_range.length, 2);
        XCTAssertEqual(called_key, output_connector_key);
        XCTAssertEqual(called_ch_idx, ch_idx);

        XCTAssertTrue(stream.has_channel(ch_idx));
        auto const &raw = get_raw<double>((*stream.channel(ch_idx).datas().begin()).second);
        XCTAssertEqual(raw.size(), 2);
        XCTAssertEqual(raw.at(0), 1.0);
        XCTAssertEqual(raw.at(1), 1.0);
    }

    {
        clear();

        process_time_range.start_frame = 4;
        stream = processing::stream{process_time_range};

        module.process(stream);

        XCTAssertEqual(called_time_range.start_frame, 4);
        XCTAssertEqual(called_time_range.length, 1);
        XCTAssertEqual(called_key, output_connector_key);
        XCTAssertEqual(called_ch_idx, ch_idx);

        XCTAssertTrue(stream.has_channel(ch_idx));
        auto const &raw = get_raw<double>((*stream.channel(ch_idx).datas().begin()).second);
        XCTAssertEqual(raw.size(), 1);
        XCTAssertEqual(raw.at(0), 1.0);
    }

    {
        clear();

        process_time_range.start_frame = 6;
        stream = processing::stream{process_time_range};

        module.process(stream);

        XCTAssertEqual(called_time_range.length, 0);
        XCTAssertTrue(called_key.empty());
        XCTAssertEqual(called_ch_idx, 0);

        XCTAssertFalse(stream.has_channel(ch_idx));
    }
}

- (void)test_process_receive_signal {
    auto const ch_idx = 7;
    auto const input_connector_key = "input";

    time_range process_time_range{.length = 2};
    stream stream = nullptr;
    time_range called_time_range;
    std::string called_key;
    connector::channel_index_t called_ch_idx;
    double called_signal[2];

    auto stream_data = processing::make_data<double>(2);
    auto &stream_data_raw = get_raw<double>(stream_data);
    stream_data_raw[0] = 1.0;
    stream_data_raw[1] = 1.0;

    auto clear = [&called_time_range, &called_key, &called_ch_idx, &called_signal]() {
        called_time_range.start_frame = 0;
        called_time_range.length = 0;
        called_key.clear();
        called_ch_idx = 0;
        called_signal[0] = 0.0;
        called_signal[1] = 0.0;
    };

    auto make_stream = [&stream_data, &ch_idx](time_range const &time_range) {
        auto stream = processing::stream{time_range};
        stream.insert_channel(ch_idx);

        auto &channel = stream.channel(ch_idx);
        channel.insert_data(time_range, stream_data);

        return stream;
    };

    auto handler = [&called_time_range, &called_key, &called_ch_idx, &called_signal](
        processing::time_range const &time_range, connector::channel_index_t const ch_idx, std::string const &key,
        double const *const signal_ptr) {
        called_time_range = time_range;
        called_key = key;
        called_ch_idx = ch_idx;
        for (auto const &idx : make_each(time_range.length)) {
            called_signal[idx] = signal_ptr[idx];
        }
    };

    auto processor = make_receive_signal_processor<double>(std::move(handler));

    auto module =
        processing::module{{.time_range = {.start_frame = 1, .length = 4}, .processors = {std::move(processor)}}};
    module.connect_input(input_connector_key, ch_idx);

    {
        clear();

        process_time_range.start_frame = 0;
        stream = make_stream(process_time_range);

        module.process(stream);

        XCTAssertEqual(called_time_range.start_frame, 1);
        XCTAssertEqual(called_time_range.length, 1);
        XCTAssertEqual(called_key, input_connector_key);
        XCTAssertEqual(called_ch_idx, ch_idx);

        XCTAssertEqual(called_signal[0], 1.0);
        XCTAssertEqual(called_signal[1], 0.0);
    }

    {
        clear();

        process_time_range.start_frame = 2;
        stream = make_stream(process_time_range);

        module.process(stream);

        XCTAssertEqual(called_time_range.start_frame, 2);
        XCTAssertEqual(called_time_range.length, 2);
        XCTAssertEqual(called_key, input_connector_key);
        XCTAssertEqual(called_ch_idx, ch_idx);

        XCTAssertEqual(called_signal[0], 1.0);
        XCTAssertEqual(called_signal[1], 1.0);
    }

    {
        clear();

        process_time_range.start_frame = 4;
        stream = make_stream(process_time_range);

        module.process(stream);

        XCTAssertEqual(called_time_range.start_frame, 4);
        XCTAssertEqual(called_time_range.length, 1);
        XCTAssertEqual(called_key, input_connector_key);
        XCTAssertEqual(called_ch_idx, ch_idx);

        XCTAssertEqual(called_signal[0], 1.0);
        XCTAssertEqual(called_signal[1], 0.0);
    }

    {
        clear();

        process_time_range.start_frame = 6;
        stream = make_stream(process_time_range);

        module.process(stream);

        XCTAssertEqual(called_time_range.length, 0);
        XCTAssertTrue(called_key.empty());
        XCTAssertEqual(called_ch_idx, 0);

        XCTAssertEqual(called_signal[0], 0.0);
        XCTAssertEqual(called_signal[1], 0.0);
    }
}

- (void)test_process_receive_and_send_signal {
    auto const receive_ch_idx = 3;
    auto const send_ch_idx = 9;
    auto const output_connector_key = "output";
    auto const input_connector_key = "input";

    time_range process_time_range{.start_frame = 0, .length = 2};

    auto stream = processing::stream{process_time_range};
    stream.insert_channel(receive_ch_idx);

    auto input_stream_data = processing::make_data<int16_t>(2);
    auto &input_stream_data_raw = get_raw<int16_t>(input_stream_data);
    input_stream_data_raw[0] = 1;
    input_stream_data_raw[1] = 2;

    auto &channel = stream.channel(receive_ch_idx);
    channel.insert_data(process_time_range, input_stream_data);

    auto process_data = processing::make_data<int16_t>(2);

    auto receive_handler = [&process_data, &input_connector_key](
        processing::time_range const &time_range, connector::channel_index_t const ch_idx, std::string const &key,
        int16_t const *const signal_ptr) {
        auto &process_data_raw = get_raw<int16_t>(process_data);
        for (auto const &idx : make_each(time_range.length)) {
            process_data_raw[idx] = signal_ptr[idx] * 2;
        }
    };

    auto send_handler = [&process_data](processing::time_range const &time_range,
                                        connector::channel_index_t const ch_idx, std::string const &key,
                                        int16_t *const signal_ptr) {
        auto &process_data_raw = get_raw<int16_t>(process_data);
        for (auto const &idx : make_each(time_range.length)) {
            signal_ptr[idx] = process_data_raw[idx];
        }
    };

    auto receive_processor = make_receive_signal_processor<int16_t>(std::move(receive_handler));
    auto send_processor = processing::make_send_signal_processor<int16_t>(std::move(send_handler));
    auto module = processing::module{
        {.time_range = process_time_range, .processors = {std::move(receive_processor), std::move(send_processor)}}};
    module.connect_input(input_connector_key, receive_ch_idx);
    module.connect_output(output_connector_key, send_ch_idx);

    module.process(stream);

    XCTAssertTrue(stream.has_channel(send_ch_idx));

    auto const &send_channel = stream.channel(send_ch_idx);
    auto const &send_data_raw = get_raw<int16_t>((*send_channel.datas().begin()).second);

    XCTAssertEqual(send_data_raw.size(), 2);
    XCTAssertEqual(send_data_raw[0], 2);
    XCTAssertEqual(send_data_raw[1], 4);

    XCTAssertTrue(stream.has_channel(receive_ch_idx));

    auto const &receive_channel = stream.channel(receive_ch_idx);
    auto const &receive_data_raw = get_raw<int16_t>((*receive_channel.datas().begin()).second);

    XCTAssertEqual(receive_data_raw.size(), 2);
    XCTAssertEqual(receive_data_raw[0], 1);
    XCTAssertEqual(receive_data_raw[1], 2);
}

@end
