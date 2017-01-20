//
//  yas_processing_constant_signal_module_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_constant_signal_module_tests : XCTestCase

@end

@implementation yas_processing_constant_signal_module_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_make_module {
    double const value = 1.0;
    auto module = constant::make_signal_module(value);

    XCTAssertTrue(module);
}

- (void)test_process {
    int64_t const value = 5;

    auto module = constant::make_signal_module(value);
    module.connect_output(to_connector_index(constant::output_key::out), 0);

    processing::stream stream{sync_source{1, 2}};

    module.process({0, 2}, stream);

    XCTAssertTrue(stream.has_channel(0));

    auto const &channel = stream.channel(0);

    XCTAssertEqual(channel.events().size(), 1);

    auto const &event_pair = *channel.events().begin();
    auto const &time = event_pair.first;
    auto const signal = cast<processing::signal_event>(event_pair.second);

    XCTAssertTrue(time.type() == typeid(processing::time::range));

    auto const &time_range = time.get<processing::time::range>();

    XCTAssertEqual(time_range.frame, 0);
    XCTAssertEqual(time_range.length, 2);
    XCTAssertEqual(signal.size(), 2);

    auto const &vec = signal.vector<int64_t>();

    XCTAssertEqual(vec[0], 5);
    XCTAssertEqual(vec[1], 5);
}

@end
