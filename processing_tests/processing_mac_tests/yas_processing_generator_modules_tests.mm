//
//  yas_processing_generator_modules_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

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

- (void)test_second {
    channel_index_t const ch_idx = 8;
    sample_rate_t const sr = 8;
    length_t const process_length = sr * 2;

    stream stream{sync_source{sr, 20}};

    auto second_module = make_signal_module<double>(generator::kind::second, 0);
    second_module.connect_output(to_connector_index(generator::output::value), ch_idx);

    second_module.process(time::range{0, process_length}, stream);

    XCTAssertTrue(stream.has_channel(ch_idx));

    auto const &channel = stream.channel(ch_idx);

    auto const &time = channel.events().begin()->first;
    auto const &time_range = time.get<time::range>();

    XCTAssertEqual(time_range.frame, 0);
    XCTAssertEqual(time_range.length, process_length);

    auto const &signal = cast<signal_event>(channel.events().begin()->second);
    auto *data = signal.data<double>();

    XCTAssertEqual(data[0], 0.0);
    XCTAssertEqual(data[1], 0.125);
    XCTAssertEqual(data[2], 0.25);
    XCTAssertEqual(data[3], 0.375);
    XCTAssertEqual(data[4], 0.5);
    XCTAssertEqual(data[5], 0.625);
    XCTAssertEqual(data[6], 0.75);
    XCTAssertEqual(data[7], 0.875);
    XCTAssertEqual(data[8], 1.0);
    XCTAssertEqual(data[9], 1.125);
    XCTAssertEqual(data[10], 1.25);
    XCTAssertEqual(data[11], 1.375);
    XCTAssertEqual(data[12], 1.5);
    XCTAssertEqual(data[13], 1.625);
    XCTAssertEqual(data[14], 1.75);
    XCTAssertEqual(data[15], 1.875);
}

- (void)test_connect_output {
    auto module = make_number_module<int32_t>(1);
    connect(module, generator::output::value, 9);
    
    auto const &connectors = module.output_connectors();
    
    XCTAssertEqual(connectors.size(), 1);
    XCTAssertEqual(connectors.begin()->first, to_connector_index(generator::output::value));
    XCTAssertEqual(connectors.begin()->second.channel_index, 9);
}

- (void)test_output_to_string {
    XCTAssertEqual(to_string(generator::output::value), "value");
}


@end
