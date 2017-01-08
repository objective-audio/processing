//
//  yas_processing_number_processor_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_number_processor_tests : XCTestCase

@end

@implementation yas_processing_number_processor_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_receive_number_processor {
    struct called {
        time::frame::type frame;
        channel_index_t ch_idx;
        std::string key;
        int64_t value;
    };

    std::vector<called> called_params;

    auto processor = make_receive_number_processor<int64_t>(
        [&called_params](processing::time::frame::type const &frame, channel_index_t const ch_idx,
                         std::string const &key, int64_t const &value) {
            called called{.frame = frame, .ch_idx = ch_idx, .key = key, .value = value};
            called_params.emplace_back(std::move(called));
        });

    stream stream;

    auto &channel0 = stream.add_channel(0);
    auto &channel1 = stream.add_channel(1);

    channel0.insert_event(make_frame_time(1), number_event{int64_t(100)});
    channel0.insert_event(make_frame_time(0), number_event{int64_t(0)});
    channel0.insert_event(make_frame_time(2), number_event{int64_t(20)});

    channel1.insert_event(make_frame_time(1), number_event{int64_t(1000)});

    module module{{std::move(processor)}};
    module.connect_input("in_0", 0);
    module.connect_input("in_1", 1);

    module.process({0, 3}, stream);

    XCTAssertEqual(called_params.size(), 4);
    
    XCTAssertEqual(called_params[0].frame, 0);
    XCTAssertEqual(called_params[1].frame, 1);
    XCTAssertEqual(called_params[2].frame, 2);
    XCTAssertEqual(called_params[3].frame, 1);
    
    XCTAssertEqual(called_params[0].ch_idx, 0);
    XCTAssertEqual(called_params[1].ch_idx, 0);
    XCTAssertEqual(called_params[2].ch_idx, 0);
    XCTAssertEqual(called_params[3].ch_idx, 1);
    
    XCTAssertEqual(called_params[0].key, "in_0");
    XCTAssertEqual(called_params[1].key, "in_0");
    XCTAssertEqual(called_params[2].key, "in_0");
    XCTAssertEqual(called_params[3].key, "in_1");
    
    XCTAssertEqual(called_params[0].value, 0);
    XCTAssertEqual(called_params[1].value, 100);
    XCTAssertEqual(called_params[2].value, 20);
    XCTAssertEqual(called_params[3].value, 1000);
}

@end
