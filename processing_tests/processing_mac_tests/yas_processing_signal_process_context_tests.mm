//
//  yas_processing_signal_process_context_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing_signal_process_context.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_signal_process_context_tests : XCTestCase

@end

@implementation yas_processing_signal_process_context_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create_by_size_2 {
    signal_process_context<int8_t, 2> context;

    XCTAssertEqual(context.signals().size(), 2);
    XCTAssertEqual(context.times().size(), 2);

    XCTAssertEqual(context.signals().at(0).size(), 0);
    XCTAssertEqual(context.signals().at(1).size(), 0);
    XCTAssertFalse(context.times().at(0));
    XCTAssertFalse(context.times().at(1));
}

- (void)test_set_time {
    signal_process_context<int8_t, 2> context;

    context.set_time(make_range_time(0, 1), 0);

    XCTAssertTrue(context.times().at(0));
    XCTAssertEqual(context.times().at(0).get<time::range>(), time::range(0, 1));
    XCTAssertFalse(context.times().at(1));

    context.set_time(make_range_time(1, 2), 1);

    XCTAssertTrue(context.times().at(0));
    XCTAssertEqual(context.times().at(0).get<time::range>(), time::range(0, 1));
    XCTAssertTrue(context.times().at(1));
    XCTAssertEqual(context.times().at(1).get<time::range>(), time::range(1, 2));
}

- (void)test_time {
    signal_process_context<int8_t, 2> context;

    XCTAssertFalse(context.time(0));
    XCTAssertFalse(context.time(1));

    context.set_time(make_range_time(0, 1), 0);
    context.set_time(make_range_time(1, 2), 1);

    XCTAssertEqual(context.time(0), make_range_time(0, 1));
    XCTAssertEqual(context.time(1), make_range_time(1, 2));
}

- (void)test_copy_data_from {
    signal_process_context<int8_t, 2> context;

    int8_t data0[2] = {11, 12};
    context.copy_data_from(data0, 2, 0);

    XCTAssertEqual(context.signals().at(0).size(), 2);
    XCTAssertEqual(context.signals().at(0).data<int8_t>()[0], 11);
    XCTAssertEqual(context.signals().at(0).data<int8_t>()[1], 12);
    XCTAssertEqual(context.signals().at(1).size(), 0);

    int8_t data1[3] = {21, 22, 23};
    context.copy_data_from(data1, 3, 1);

    XCTAssertEqual(context.signals().at(0).size(), 2);
    XCTAssertEqual(context.signals().at(0).data<int8_t>()[0], 11);
    XCTAssertEqual(context.signals().at(0).data<int8_t>()[1], 12);
    XCTAssertEqual(context.signals().at(1).size(), 3);
    XCTAssertEqual(context.signals().at(1).data<int8_t>()[0], 21);
    XCTAssertEqual(context.signals().at(1).data<int8_t>()[1], 22);
    XCTAssertEqual(context.signals().at(1).data<int8_t>()[2], 23);
}

- (void)test_data {
    signal_process_context<int8_t, 2> context;

    int8_t data0[2] = {11, 12};
    context.copy_data_from(data0, 2, 0);
    int8_t data1[3] = {21, 22, 23};
    context.copy_data_from(data1, 3, 1);

    XCTAssertEqual(context.data(0)[0], 11);
    XCTAssertEqual(context.data(0)[1], 12);

    XCTAssertEqual(context.data(1)[0], 21);
    XCTAssertEqual(context.data(1)[1], 22);
    XCTAssertEqual(context.data(1)[2], 23);
}

- (void)test_reset {
    signal_process_context<int8_t, 1> context;

    int8_t data[1] = {100};
    context.copy_data_from(data, 1, 0);

    context.set_time(make_range_time(10, 1), 0);

    context.reset(2);

    XCTAssertEqual(context.signals().at(0).size(), 0);
    XCTAssertFalse(context.times().at(0));
}

@end
