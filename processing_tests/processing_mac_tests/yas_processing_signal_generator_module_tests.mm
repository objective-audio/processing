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
    auto module = processing::make_signal_generator_module<double>(time_range{}, nullptr);

    XCTAssertTrue(module);
}

- (void)test_create_null {
    signal_generator_module module = nullptr;

    XCTAssertFalse(module);
}

- (void)test_process_signal {
    auto const ch_idx = 5;

    time_range process_time_range{.length = 2};
    stream stream = nullptr;
    time_range called_time_range;
    double called_signal[2];

    auto clear = [&called_time_range, &called_signal]() {
        called_time_range.start_frame = 0;
        called_time_range.length = 0;
        called_signal[0] = 0.0;
        called_signal[1] = 0.0;
    };

    auto handler = [&called_time_range, &called_signal](processing::time_range const &time_range, int64_t const ch_idx, double *signal_ptr) {
        called_time_range = time_range;
        for (auto const &idx : make_each(time_range.length)) {
            signal_ptr[idx] = 1.0;
        }
    };

    auto module = make_signal_generator_module<double>({.start_frame = 1, .length = 4}, std::move(handler));
    module.connect_output(signal_generator_module::output_key, ch_idx);

    {
        clear();

        process_time_range.start_frame = 0;
        stream = processing::stream{process_time_range};

        module.process(stream);

        XCTAssertEqual(called_time_range.start_frame, 1);
        XCTAssertEqual(called_time_range.length, 1);

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

        XCTAssertFalse(stream.has_channel(ch_idx));
    }
}

@end
