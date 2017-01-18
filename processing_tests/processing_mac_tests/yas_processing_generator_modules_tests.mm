//
//  yas_processing_generator_modules_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

#import <iostream>

using namespace yas;
using namespace yas::processing;

@interface yas_processing_generator_modules_tests : XCTestCase

@end

@implementation yas_processing_generator_modules_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_sine {
    channel_index_t const ch_idx = 3;

    auto sine_module = make_signal_module<double>(generator::kind::sin);
    sine_module.connect_output(to_connector_index(generator::output_key::out), ch_idx);
    sine_module.connect_input(to_connector_index(generator::input_key::phase), ch_idx);

    stream stream;

    {
        auto &channel = stream.add_channel(ch_idx);

        auto phase_signal = make_signal_event<double>(8);
        auto *phase_data = phase_signal.data<double>();
        auto const two_pi = 2.0 * M_PI;
        phase_data[0] = 0.0;
        phase_data[1] = 0.25 * two_pi;
        phase_data[2] = 0.5 * two_pi;
        phase_data[3] = 0.75 * two_pi;
        phase_data[4] = 1.0 * two_pi;
        phase_data[5] = 1.25 * two_pi;
        phase_data[6] = 1.5 * two_pi;
        phase_data[7] = 1.75 * two_pi;

        channel.insert_event(make_range_time(0, 8), std::move(phase_signal));
    }

    sine_module.process(time::range{0, 8}, stream);

    {
        auto const &events = stream.channel(ch_idx).events();

        XCTAssertEqual(events.size(), 1);

        auto const signal = cast<processing::signal_event>((*events.begin()).second);
        auto const &vec = signal.vector<double>();

        XCTAssertEqualWithAccuracy(vec[0], 0.0, 0.01);
        XCTAssertEqualWithAccuracy(vec[1], 1.0, 0.01);
        XCTAssertEqualWithAccuracy(vec[2], 0.0, 0.01);
        XCTAssertEqualWithAccuracy(vec[3], -1.0, 0.01);
        XCTAssertEqualWithAccuracy(vec[4], 0.0, 0.01);
        XCTAssertEqualWithAccuracy(vec[5], 1.0, 0.01);
        XCTAssertEqualWithAccuracy(vec[6], 0.0, 0.01);
        XCTAssertEqualWithAccuracy(vec[7], -1.0, 0.01);
    }
}

@end
