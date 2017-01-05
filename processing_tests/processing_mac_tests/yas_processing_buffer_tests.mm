//
//  yas_processing_buffer_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing_buffer.h"
#import "yas_each_index.h"
#import <string>

using namespace yas;
using namespace yas::processing;

@interface yas_processing_buffer_tests : XCTestCase

@end

@implementation yas_processing_buffer_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_make_buffer {
    auto buffer = processing::make_buffer<float>(16);

    XCTAssertEqual(buffer.sample_byte_count(), 4);
    XCTAssertEqual(buffer.size(), 16);

    XCTAssertEqual(buffer.vector<float>().size(), 16);

    for (auto const &idx : make_each(16)) {
        buffer.vector<float>()[idx] = static_cast<float>(idx);
    }

    for (auto const &idx : make_each(16)) {
        XCTAssertEqual(buffer.vector<float>()[idx], static_cast<float>(idx));
    }
}

- (void)test_make_buffer_with_reserve {
    auto buffer = processing::make_buffer<float>(8, 16);

    XCTAssertEqual(buffer.vector<float>().size(), 8);
    XCTAssertEqual(buffer.vector<float>().capacity(), 16);
}

- (void)test_create_buffer_with_moved_vector {
    buffer buffer{std::vector<double>{0.0, 2.0, 4.0, 8.0}};

    XCTAssertEqual(buffer.sample_byte_count(), 8);
    XCTAssertEqual(buffer.size(), 4);

    XCTAssertEqual(buffer.vector<double>().size(), 4);

    XCTAssertEqual(buffer.vector<double>()[0], 0.0);
    XCTAssertEqual(buffer.vector<double>()[1], 2.0);
    XCTAssertEqual(buffer.vector<double>()[2], 4.0);
    XCTAssertEqual(buffer.vector<double>()[3], 8.0);
}

- (void)test_create_buffer_with_reference_vector {
    std::vector<int32_t> vec(3);

    buffer buffer{vec};

    vec[0] = 5;
    vec[1] = 6;
    vec[2] = 7;

    XCTAssertEqual(buffer.sample_byte_count(), 4);
    XCTAssertEqual(buffer.size(), 3);

    XCTAssertEqual(buffer.vector<int32_t>().size(), 3);
    XCTAssertTrue(vec.data() == buffer.vector<int32_t>().data());

    XCTAssertEqual(buffer.vector<int32_t>()[0], 5);
    XCTAssertEqual(buffer.vector<int32_t>()[1], 6);
    XCTAssertEqual(buffer.vector<int32_t>()[2], 7);
}

- (void)test_create_buffer_with_struct {
    struct element {
        std::string key;
        uint32_t value;
    };

    auto buffer = make_buffer<element>(2);
    buffer.vector<element>()[0].key = "zero";
    buffer.vector<element>()[0].value = 100;
    buffer.vector<element>()[1].key = "one";
    buffer.vector<element>()[1].value = 200;

    XCTAssertEqual(buffer.sample_byte_count(), sizeof(element));
    XCTAssertEqual(buffer.size(), 2);

    XCTAssertEqual(buffer.vector<element>().size(), 2);

    XCTAssertEqual(buffer.vector<element>()[0].key, "zero");
    XCTAssertEqual(buffer.vector<element>()[0].value, 100);
    XCTAssertEqual(buffer.vector<element>()[1].key, "one");
    XCTAssertEqual(buffer.vector<element>()[1].value, 200);
}

- (void)test_get_data {
    auto buffer = make_buffer<int16_t>(2);

    auto *data = buffer.data<int16_t>();
    data[0] = 1000;
    data[1] = 1001;

    auto const *const_data = buffer.data<int16_t>();
    XCTAssertEqual(const_data[0], 1000);
    XCTAssertEqual(const_data[1], 1001);
}

- (void)test_get_vector {
    auto buffer = make_buffer<int16_t>(2);

    auto &vec = buffer.vector<int16_t>();
    vec[0] = 200;
    vec[1] = 210;

    auto const &const_vec = buffer.vector<int16_t>();
    XCTAssertEqual(const_vec[0], 200);
    XCTAssertEqual(const_vec[1], 210);
}

@end
