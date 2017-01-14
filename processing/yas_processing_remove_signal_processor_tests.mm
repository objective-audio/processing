//
//  yas_processing_remove_signal_processor_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_remove_signal_processor_tests : XCTestCase

@end

@implementation yas_processing_remove_signal_processor_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_process {
    stream stream;

    auto &channel = stream.add_channel(0);
    channel.insert_event(make_range_time(0, 1), make_signal_event<int16_t>(1));

    module module{{make_remove_signal_processor<int16_t>()}};
    module.connect_input("in", 0);

    module.process({0, 1}, stream);

    XCTAssertEqual(channel.events().size(), 0);
}

- (void)test_process_range {
    stream stream;

    auto &channel = stream.add_channel(0);
    auto signal = make_signal_event<int16_t>(3);
    signal.data<int16_t>()[0] = 100;
    signal.data<int16_t>()[1] = 101;
    signal.data<int16_t>()[2] = 102;
    channel.insert_event(make_range_time(0, 3), signal);

    module module{{make_remove_signal_processor<int16_t>()}};
    module.connect_input("in", 0);

    module.process({1, 1}, stream);

    XCTAssertEqual(channel.events().size(), 2);

    auto it = channel.events().begin();
    XCTAssertEqual((*it).first.get<time::range>().frame, 0);
    XCTAssertEqual((*it).first.get<time::range>().length, 1);
    XCTAssertEqual(cast<signal_event>((*it).second).data<int16_t>()[0], 100);

    ++it;
    XCTAssertEqual((*it).first.get<time::range>().frame, 2);
    XCTAssertEqual((*it).first.get<time::range>().length, 1);
    XCTAssertEqual(cast<signal_event>((*it).second).data<int16_t>()[0], 102);
}

- (void)test_process_type {
    stream stream;

    auto &channel = stream.add_channel(0);
    channel.insert_event(make_range_time(0, 1), make_signal_event<int8_t>(1));
    channel.insert_event(make_range_time(0, 1), make_signal_event<int16_t>(1));
    channel.insert_event(make_range_time(0, 1), make_signal_event<int32_t>(1));

    module module{{make_remove_signal_processor<int16_t>()}};
    module.connect_input("in", 0);

    module.process({0, 1}, stream);

    XCTAssertEqual(channel.events().size(), 2);

    auto it = channel.events().begin();
    XCTAssertTrue(cast<signal_event>((*it).second).sample_type() != typeid(int16_t));
    ++it;
    XCTAssertTrue(cast<signal_event>((*it).second).sample_type() != typeid(int16_t));
}

@end
