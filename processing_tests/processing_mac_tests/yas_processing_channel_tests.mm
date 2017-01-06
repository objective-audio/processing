//
//  yas_processing_channel_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_channel_tests : XCTestCase

@end

@implementation yas_processing_channel_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create_channel {
    processing::channel channel;

    XCTAssertTrue(channel);
    XCTAssertEqual(channel.events().size(), 0);
}

- (void)test_create_null {
    processing::channel channel = nullptr;

    XCTAssertFalse(channel);
}

- (void)test_insert_event {
    processing::channel channel;

    auto uint_signal = make_signal_event<uint32_t>(4);
    auto &uint_vec = uint_signal.vector<uint32_t>();
    uint_vec[0] = 10;
    uint_vec[1] = 11;
    uint_vec[2] = 12;
    uint_vec[3] = 13;

    auto float_signal = make_signal_event<float>(2);
    auto &float_vec = float_signal.vector<float>();
    float_vec[0] = 2.0f;
    float_vec[1] = 4.0f;

    processing::time uint_range_time{16, 4};
    processing::time float_range_time{8, 2};

    channel.insert_event(std::move(uint_range_time), signal_event{std::move(uint_vec)});
    channel.insert_event(std::move(float_range_time), signal_event{std::move(float_signal)});

    std::size_t idx = 0;
    for (auto &pair : channel.events()) {
        auto const &time = pair.first;
        auto const &event = pair.second;
        if (idx == 0) {
            XCTAssertTrue((time == processing::time{8, 2}));
            auto const signal = cast<processing::signal_event>(event);
            XCTAssertTrue(signal.sample_type() == typeid(float));
            auto const &vec = signal.vector<float>();
            XCTAssertEqual(vec.size(), 2);
            XCTAssertEqual(vec[0], 2.0f);
            XCTAssertEqual(vec[1], 4.0f);
        } else if (idx == 1) {
            XCTAssertTrue((time == processing::time{16, 4}));
            auto const signal = cast<processing::signal_event>(event);
            XCTAssertTrue(signal.sample_type() == typeid(uint32_t));
            auto const &vec = signal.vector<uint32_t>();
            XCTAssertEqual(vec.size(), 4);
            XCTAssertEqual(vec[0], 10);
            XCTAssertEqual(vec[1], 11);
            XCTAssertEqual(vec[2], 12);
            XCTAssertEqual(vec[3], 13);
        }

        ++idx;
    }
}

- (void)test_insert_same_time_range_signal_event {
    processing::channel channel;

    auto uint_signal = make_signal_event<uint32_t>(2);
    processing::time uint_range_time{1, 2};

    auto float_signal = make_signal_event<float>(2);
    processing::time float_range_time{1, 2};

    channel.insert_event(uint_range_time, uint_signal);
    channel.insert_event(float_range_time, float_signal);

    XCTAssertEqual(channel.events().size(), 2);
}

- (void)test_const_signal_event {
    processing::channel channel;

    auto signal_event = make_signal_event<float>(1);
    signal_event.vector<float>()[0] = 1.0f;

    channel.insert_event(processing::time{10, 1}, std::move(signal_event));

    processing::channel const &const_channel = channel;

    XCTAssertEqual(const_channel.events().size(), 1);

    for (auto const &pair : const_channel.events()) {
        auto const &const_time = pair.first;
        auto const &const_signal = cast<processing::signal_event>(pair.second);

        XCTAssertTrue((const_time == processing::time{10, 1}));
        XCTAssertEqual(const_signal.vector<float>()[0], 1.0f);
    }
}

- (void)test_invalid_insert_event {
    processing::channel channel;

    auto signal_event = make_signal_event<float>(2);

    XCTAssertThrows(channel.insert_event(processing::time{0, 1}, std::move(signal_event)));
}

@end
