//
//  yas_processing_module_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing_test_utils.h"

using namespace yas;
using namespace yas::proc;

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
    proc::module module{[] { return module::processors_t{}; }};

    XCTAssertTrue(module);
    XCTAssertEqual(module.input_connectors().size(), 0);
    XCTAssertEqual(module.output_connectors().size(), 0);
}

- (void)test_create_null {
    proc::module module = nullptr;

    XCTAssertFalse(module);
}

- (void)test_process_called {
    proc::stream stream{sync_source{1, 456}};

    proc::time called_time = nullptr;

    proc::module module{[&called_time] {
        auto processor = [&called_time](proc::time::range const &time_range, auto const &, auto const &,
                                        proc::stream &stream) { called_time = proc::time{time_range}; };
        return module::processors_t{{std::move(processor)}};
    }};

    module.process({23, 456}, stream);

    XCTAssertEqual(called_time.get<time::range>().frame, 23);
    XCTAssertEqual(called_time.get<time::range>().length, 456);
}

- (void)test_connect_input {
    connector_index_t const co_idx = 10;

    proc::module module{[] { return module::processors_t{}; }};

    module.connect_input(co_idx, 1);

    XCTAssertEqual(module.input_connectors().size(), 1);
    XCTAssertEqual(module.input_connectors().count(co_idx), 1);
    XCTAssertEqual(module.input_connectors().at(co_idx).channel_index, 1);
}

- (void)test_connect_output {
    connector_index_t const co_idx = 20;

    proc::module module{[] { return module::processors_t{}; }};

    module.connect_output(co_idx, 2);

    XCTAssertEqual(module.output_connectors().size(), 1);
    XCTAssertEqual(module.output_connectors().count(co_idx), 1);
    XCTAssertEqual(module.output_connectors().at(co_idx).channel_index, 2);
}

- (void)test_disconnect_input {
    connector_index_t const co_idx_a = 30;
    connector_index_t const co_idx_b = 31;

    proc::module module{[] { return module::processors_t{}; }};

    module.connect_input(co_idx_a, 3);

    module.disconnect_input(co_idx_b);

    XCTAssertEqual(module.input_connectors().size(), 1);

    module.disconnect_input(co_idx_a);

    XCTAssertEqual(module.input_connectors().size(), 0);
}

- (void)test_disconnect_output {
    connector_index_t const co_idx_a = 40;
    connector_index_t const co_idx_b = 41;

    proc::module module{[] { return module::processors_t{}; }};

    module.connect_output(co_idx_a, 3);

    module.disconnect_output(co_idx_b);

    XCTAssertEqual(module.output_connectors().size(), 1);

    module.disconnect_output(co_idx_a);

    XCTAssertEqual(module.output_connectors().size(), 0);
}

- (void)test_copy {
    std::vector<int> called;

    auto index = std::make_shared<int>(0);
    proc::module module{[index = std::move(index), &called] {
        auto processor = [index = *index, &called](time::range const &, connector_map_t const &,
                                                   connector_map_t const &, stream &) { called.push_back(index); };
        ++(*index);
        return module::processors_t{std::move(processor)};
    }};

    auto copied_module = module.copy();

    proc::stream stream{sync_source{1, 1}};

    module.process({0, 1}, stream);

    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), 0);

    copied_module.process({0, 1}, stream);

    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), 1);
}

@end
