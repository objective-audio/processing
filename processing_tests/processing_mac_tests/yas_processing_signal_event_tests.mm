//
//  yas_processing_signal_event_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing_signal_event.h"
#import "yas_each_index.h"
#import <string>

using namespace yas;
using namespace yas::processing;

@interface yas_processing_signal_event_tests : XCTestCase

@end

@implementation yas_processing_signal_event_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_make_signal_event {
    auto signal_event = processing::make_signal_event<float>(16);

    XCTAssertEqual(signal_event.sample_byte_count(), 4);
    XCTAssertEqual(signal_event.size(), 16);

    XCTAssertEqual(signal_event.vector<float>().size(), 16);

    for (auto const &idx : make_each(16)) {
        signal_event.vector<float>()[idx] = static_cast<float>(idx);
    }

    for (auto const &idx : make_each(16)) {
        XCTAssertEqual(signal_event.vector<float>()[idx], static_cast<float>(idx));
    }
}

- (void)test_make_signal_event_with_reserve {
    auto signal_event = processing::make_signal_event<float>(8, 16);

    XCTAssertEqual(signal_event.vector<float>().size(), 8);
    XCTAssertEqual(signal_event.vector<float>().capacity(), 16);
}

- (void)test_create_signal_event_with_moved_vector {
    signal_event signal_event{std::vector<double>{0.0, 2.0, 4.0, 8.0}};

    XCTAssertEqual(signal_event.sample_byte_count(), 8);
    XCTAssertEqual(signal_event.size(), 4);

    XCTAssertEqual(signal_event.vector<double>().size(), 4);

    XCTAssertEqual(signal_event.vector<double>()[0], 0.0);
    XCTAssertEqual(signal_event.vector<double>()[1], 2.0);
    XCTAssertEqual(signal_event.vector<double>()[2], 4.0);
    XCTAssertEqual(signal_event.vector<double>()[3], 8.0);
}

- (void)test_create_signal_event_with_reference_vector {
    std::vector<int32_t> vec(3);

    signal_event signal_event{vec};

    vec[0] = 5;
    vec[1] = 6;
    vec[2] = 7;

    XCTAssertEqual(signal_event.sample_byte_count(), 4);
    XCTAssertEqual(signal_event.size(), 3);

    XCTAssertEqual(signal_event.vector<int32_t>().size(), 3);
    XCTAssertTrue(vec.data() == signal_event.vector<int32_t>().data());

    XCTAssertEqual(signal_event.vector<int32_t>()[0], 5);
    XCTAssertEqual(signal_event.vector<int32_t>()[1], 6);
    XCTAssertEqual(signal_event.vector<int32_t>()[2], 7);
}

- (void)test_create_signal_event_with_struct {
    struct element {
        std::string key;
        uint32_t value;
    };

    auto signal_event = make_signal_event<element>(2);
    signal_event.vector<element>()[0].key = "zero";
    signal_event.vector<element>()[0].value = 100;
    signal_event.vector<element>()[1].key = "one";
    signal_event.vector<element>()[1].value = 200;

    XCTAssertEqual(signal_event.sample_byte_count(), sizeof(element));
    XCTAssertEqual(signal_event.size(), 2);

    XCTAssertEqual(signal_event.vector<element>().size(), 2);

    XCTAssertEqual(signal_event.vector<element>()[0].key, "zero");
    XCTAssertEqual(signal_event.vector<element>()[0].value, 100);
    XCTAssertEqual(signal_event.vector<element>()[1].key, "one");
    XCTAssertEqual(signal_event.vector<element>()[1].value, 200);
}

- (void)test_get_data {
    auto signal_event = make_signal_event<int16_t>(2);

    auto *data = signal_event.data<int16_t>();
    data[0] = 1000;
    data[1] = 1001;

    auto const *const_data = signal_event.data<int16_t>();
    XCTAssertEqual(const_data[0], 1000);
    XCTAssertEqual(const_data[1], 1001);
}

- (void)test_get_vector {
    auto signal_event = make_signal_event<int16_t>(2);

    auto &vec = signal_event.vector<int16_t>();
    vec[0] = 200;
    vec[1] = 210;

    auto const &const_vec = signal_event.vector<int16_t>();
    XCTAssertEqual(const_vec[0], 200);
    XCTAssertEqual(const_vec[1], 210);
}

- (void)test_resize {
    auto signal_event = make_signal_event<int16_t>(2);

    XCTAssertEqual(signal_event.size(), 2);

    signal_event.resize(16);

    XCTAssertEqual(signal_event.size(), 16);

    signal_event.resize(0);

    XCTAssertEqual(signal_event.size(), 0);
}

- (void)test_sample_byte_count {
    XCTAssertEqual(processing::make_signal_event<int8_t>(1).sample_byte_count(), 1);
    XCTAssertEqual(processing::make_signal_event<double>(1).sample_byte_count(), 8);
    XCTAssertEqual(processing::make_signal_event<processing::boolean>(1).sample_byte_count(), 1);
}

- (void)test_sample_type {
    XCTAssertTrue(processing::make_signal_event<int8_t>(1).sample_type() == typeid(int8_t));
    XCTAssertTrue(processing::make_signal_event<double>(0.0).sample_type() == typeid(double));
    XCTAssertTrue(processing::make_signal_event<processing::boolean>(1).sample_type() == typeid(processing::boolean));
}

- (void)test_copy_from {
    auto signal_event = make_signal_event<int16_t>(0);

    std::vector<int16_t> vec{100, 101};

    signal_event.copy_from(vec.data(), vec.size());

    XCTAssertEqual(signal_event.data<int16_t>()[0], 100);
    XCTAssertEqual(signal_event.data<int16_t>()[1], 101);
}

- (void)test_copy_to {
    auto signal_event = make_signal_event<int16_t>(2);
    signal_event.data<int16_t>()[0] = 1000;
    signal_event.data<int16_t>()[1] = 1001;

    std::vector<int16_t> vec(2);

    signal_event.copy_to(vec.data(), vec.size());

    XCTAssertEqual(vec[0], 1000);
    XCTAssertEqual(vec[1], 1001);
}

- (void)test_boolean_signal_event {
    auto signal_event = make_signal_event<processing::boolean>(2);
    
    auto &vec = signal_event.vector<processing::boolean>();
    vec[0].value = true;
    vec[1].value = false;
    
    auto const *data = signal_event.data<processing::boolean>();
    
    XCTAssertTrue(data[0].value);
    XCTAssertFalse(data[1].value);
}

@end
