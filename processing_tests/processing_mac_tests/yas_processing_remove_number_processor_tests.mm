//
//  yas_processing_remove_number_processor_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_remove_number_processor_tests : XCTestCase

@end

@implementation yas_processing_remove_number_processor_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_process {
    stream stream;

    auto &channel0 = stream.add_channel(0);
    channel0.insert_event(make_frame_time(0), processing::number_event{int8_t(0)});
    channel0.insert_event(make_frame_time(0), processing::number_event{float(1.0f)});
    channel0.insert_event(make_frame_time(10), processing::number_event{int8_t(10)});

    auto &channel1 = stream.add_channel(1);
    channel1.insert_event(make_frame_time(0), processing::number_event{int8_t(0)});

    module module{{make_remove_number_processor<int8_t>({"in"})}};

    module.connect_input("in", 0);

    module.process({0, 1}, stream);

    XCTAssertEqual(channel0.events().size(), 2);
    XCTAssertEqual(channel1.events().size(), 1);
}

- (void)test_key {
    stream stream;

    {
        auto &channel0 = stream.add_channel(0);
        channel0.insert_event(make_frame_time(0), make_number_event<int8_t>(0));

        auto &channel1 = stream.add_channel(1);
        channel1.insert_event(make_frame_time(0), make_number_event<int8_t>(1));

        auto &channel2 = stream.add_channel(2);
        channel2.insert_event(make_frame_time(0), make_number_event<int8_t>(2));
    }

    module module{{make_remove_number_processor<int8_t>({"in_a", "in_c"})}};
    module.connect_input("in_a", 0);
    module.connect_input("in_b", 1);
    module.connect_input("in_c", 2);

    module.process({0, 1}, stream);

    {
        auto &channel0 = stream.channel(0);
        XCTAssertEqual(channel0.events().size(), 0);

        auto &channel1 = stream.channel(1);
        XCTAssertEqual(channel1.events().size(), 1);

        auto &channel2 = stream.channel(2);
        XCTAssertEqual(channel2.events().size(), 0);
    }
}

@end
