//
//  yas_processing_signal_generator_module_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_each_index.h"
#import "yas_processing_test_utils.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_signal_generator_module_tests : XCTestCase

@end

@implementation yas_processing_signal_generator_module_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create {
    auto module = processing::make_signal_generator_module<double>({});

    XCTAssertTrue(module);
}

- (void)test_create_null {
    signal_generator_module module = nullptr;

    XCTAssertFalse(module);
}

- (void)test_process_send_signal {
    auto const ch_idx = 5;
    auto const output_connector_key = "output";

    time_range process_time_range{.length = 2};
    stream stream = nullptr;
    time_range called_time_range;
    std::string called_key;
    int64_t called_ch_idx;

    auto clear = [&called_time_range, &called_key, &called_ch_idx]() {
        called_time_range.start_frame = 0;
        called_time_range.length = 0;
        called_key.clear();
        called_ch_idx = 0;
    };

    auto handler = [&called_time_range, &called_key, &called_ch_idx](processing::time_range const &time_range,
                                                                     int64_t const ch_idx, std::string const &key,
                                                                     double *const signal_ptr) {
        called_time_range = time_range;
        called_key = key;
        called_ch_idx = ch_idx;
        for (auto const &idx : make_each(time_range.length)) {
            signal_ptr[idx] = 1.0;
        }
    };

    auto module = make_signal_generator_module<double>(
        {.time_range = {.start_frame = 1, .length = 4}, .send_signal_handler = std::move(handler)});
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
    int64_t called_ch_idx;
    double called_signal[2];

    auto stream_data = processing::make_data<double>(2);
    auto &straem_data_raw = get_raw<double>(stream_data);
    straem_data_raw[0] = 1.0;
    straem_data_raw[1] = 1.0;

    auto clear = [&called_time_range, &called_key, &called_ch_idx, &called_signal]() {
        called_time_range.start_frame = 0;
        called_time_range.length = 0;
        called_key.clear();
        called_ch_idx = 0;
        called_signal[0] = 0.0;
        called_signal[1] = 0.0;
    };

    auto make_stream = [&straem_data_raw, &ch_idx](time_range const &time_range) {
        auto stream = processing::stream{time_range};
        stream.insert_channel(ch_idx);

        auto &channel = stream.channel(ch_idx);
        channel.insert_data(time_range, straem_data_raw);

        return stream;
    };

    auto handler = [&called_time_range, &called_key, &called_ch_idx, &called_signal](
        processing::time_range const &time_range, int64_t const ch_idx, std::string const &key,
        double const *const signal_ptr) {
        called_time_range = time_range;
        called_key = key;
        called_ch_idx = ch_idx;
        for (auto const &idx : make_each(time_range.length)) {
            called_signal[idx] = signal_ptr[idx];
        }
    };

    auto module = make_signal_generator_module<double>(
        {.time_range = {.start_frame = 1, .length = 4}, .receive_signal_handler = std::move(handler)});
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

@end
