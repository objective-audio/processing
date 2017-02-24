//
//  yas_processing_module_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing_test_utils.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_module_tests : XCTestCase

@end

@implementation yas_processing_module_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create_module {
    processing::module module{processing::module::processors_t{}};

    XCTAssertTrue(module);
    XCTAssertEqual(module.input_connectors().size(), 0);
    XCTAssertEqual(module.output_connectors().size(), 0);
}

- (void)test_create_null {
    processing::module module = nullptr;

    XCTAssertFalse(module);
}

- (void)test_process_called {
    processing::stream stream{sync_source{1, 456}};

    processing::time called_time = nullptr;

    auto processor = [&called_time](processing::time::range const &time_range, auto const &, auto const &,
                                    processing::stream &stream) { called_time = processing::time{time_range}; };
    processing::module module{{std::move(processor)}};

    module.process({23, 456}, stream);

    XCTAssertEqual(called_time.get<time::range>().frame, 23);
    XCTAssertEqual(called_time.get<time::range>().length, 456);
}

- (void)test_connect_input {
    connector_index_t const co_idx = 10;

    processing::module module{processing::module::processors_t{}};

    module.connect_input(co_idx, 1);

    XCTAssertEqual(module.input_connectors().size(), 1);
    XCTAssertEqual(module.input_connectors().count(co_idx), 1);
    XCTAssertEqual(module.input_connectors().at(co_idx).channel_index, 1);
}

- (void)test_connect_output {
    connector_index_t const co_idx = 20;

    processing::module module{processing::module::processors_t{}};

    module.connect_output(co_idx, 2);

    XCTAssertEqual(module.output_connectors().size(), 1);
    XCTAssertEqual(module.output_connectors().count(co_idx), 1);
    XCTAssertEqual(module.output_connectors().at(co_idx).channel_index, 2);
}

- (void)test_disconnect_input {
    connector_index_t const co_idx_a = 30;
    connector_index_t const co_idx_b = 31;

    processing::module module{processing::module::processors_t{}};

    module.connect_input(co_idx_a, 3);

    module.disconnect_input(co_idx_b);

    XCTAssertEqual(module.input_connectors().size(), 1);

    module.disconnect_input(co_idx_a);

    XCTAssertEqual(module.input_connectors().size(), 0);
}

- (void)test_disconnect_output {
    connector_index_t const co_idx_a = 40;
    connector_index_t const co_idx_b = 41;

    processing::module module{processing::module::processors_t{}};

    module.connect_output(co_idx_a, 3);

    module.disconnect_output(co_idx_b);

    XCTAssertEqual(module.output_connectors().size(), 1);

    module.disconnect_output(co_idx_a);

    XCTAssertEqual(module.output_connectors().size(), 0);
}

@end
