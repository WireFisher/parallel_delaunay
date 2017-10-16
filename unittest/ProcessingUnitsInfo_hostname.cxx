#include "mpi.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../processing_unit_mgt.h"
#include "../delaunay_grid_decomposition_mgt.h"

Grid_info_manager *grid_info_mgr;
Process_thread_manager *process_thread_mgr;

class Mock_Process_thread_manager : public Process_thread_manager//_base
{
public:
    MOCK_METHOD2(get_hostname, void(char* hostname, int len));
    MOCK_METHOD0(get_mpi_rank, int());
    MOCK_METHOD0(get_mpi_size, int());
    MOCK_METHOD0(get_mpi_comm, MPI_Comm());
    MOCK_METHOD0(get_openmp_size, int());
    //MOCK_METHOD7(allgather, int(void*, int, MPI_Datatype, void*, int, MPI_Datatype, MPI_Comm));
};

using ::testing::Return;
using ::testing::NiceMock;
using ::testing::_;
using ::testing::Invoke;


int mpi_rank = -1;
int mpi_size = 0;

void get_default_hostname(char* hostname, int len)
{
    memcpy(hostname, "default", len);
}

void get_empty_hostname(char* hostname, int len)
{
    memcpy(hostname, "", len);
}

void get_different_hostname(char* hostname, int len)
{
    if(mpi_rank < 0)
        ASSERT_TRUE(false);
    else if(mpi_rank < mpi_size * 0.25)
        memcpy(hostname, "default", len);
    else if(mpi_rank < mpi_size * 0.5)
        memcpy(hostname, "default", len);
    else if(mpi_rank < mpi_size * 0.75)
        memcpy(hostname, "default", len);
    else if(mpi_rank < mpi_size)
        memcpy(hostname, "default", len);
    else
        ASSERT_TRUE(false);
}

/*
int allgather_num_threads(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
    int* buf = (int*)recvbuf;
    for(int i = 0; i < mpi_size; i++)
        buf[i] = 4;
}

int allgather_hostname_checksum(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm)
{
    unsigned int* buf = (unsigned int*)recvbuf;
    for(int i = 0; i < mpi_size; i++)
        buf[i] = 567;
}
    
    ON_CALL(*mock_process_thread_manager, allgather(_, _, MPI_INT, _, _, _, _))
        .WillByDefault(Invoke(allgather_num_threads));

    ON_CALL(*mock_process_thread_manager, allgather(_, _, MPI_UNSIGNED, _, _, _, _))
        .WillByDefault(Invoke(allgather_hostname_checksum));
    */

TEST(ProcessingUnitsInfoTest, EmptyHostname) {
    NiceMock<Mock_Process_thread_manager> *mock_process_thread_manager = new NiceMock<Mock_Process_thread_manager>;
    process_thread_mgr = mock_process_thread_manager;
    grid_info_mgr = new Grid_info_manager;

    char *hostname;
    int len;
    ON_CALL(*mock_process_thread_manager, get_hostname(_, _))
        .WillByDefault(Invoke(get_empty_hostname));

    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    ON_CALL(*mock_process_thread_manager, get_mpi_rank())
        .WillByDefault(Return(mpi_rank));

    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    ON_CALL(*mock_process_thread_manager, get_mpi_size())
        .WillByDefault(Return(mpi_size));

    ON_CALL(*mock_process_thread_manager, get_mpi_comm())
        .WillByDefault(Return(MPI_COMM_WORLD));

    ON_CALL(*mock_process_thread_manager, get_openmp_size())
        .WillByDefault(Return(4));

    Processing_info *processing_info;
    Processing_unit **processing_units;
    int num_total_processing_units;
    unsigned int hostname_checksum;
    int common_id_prev;

    processing_info = new Processing_info();
    //processing_info->pick_out_actived_processing_units(1,1, 200.0);
    processing_units = processing_info->get_processing_units();
    num_total_processing_units = processing_info->get_num_total_processing_units();

    ASSERT_GT(num_total_processing_units, 0);
    ASSERT_LE(num_total_processing_units, mpi_size * 4);
    ASSERT_NE(processing_units, (void*)NULL);

    hostname_checksum = processing_units[0]->hostname_checksum;
    common_id_prev = 0;
    for(int i = 0; i < num_total_processing_units; i ++) {
        ASSERT_EQ(processing_units[i]->hostname_checksum, hostname_checksum);
        ASSERT_GE(processing_units[i]->process_id, 0);
        ASSERT_LT(processing_units[i]->process_id, mpi_size);
        ASSERT_GE(processing_units[i]->thread_id, 0);
        ASSERT_LT(processing_units[i]->thread_id, 4);
        if(i != num_total_processing_units - 1)
            ASSERT_EQ(processing_units[i]->common_id, processing_units[i+1]->common_id - 1);
    }

    delete grid_info_mgr;
    delete process_thread_mgr;
};


TEST(ProcessingUnitsInfoTest, DefaultHostname) {
    NiceMock<Mock_Process_thread_manager> *mock_process_thread_manager = new NiceMock<Mock_Process_thread_manager>;
    process_thread_mgr = mock_process_thread_manager;
    grid_info_mgr = new Grid_info_manager;

    char *hostname;
    int len;
    ON_CALL(*mock_process_thread_manager, get_hostname(_, _))
        .WillByDefault(Invoke(get_default_hostname));

    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    ON_CALL(*mock_process_thread_manager, get_mpi_rank())
        .WillByDefault(Return(mpi_rank));

    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    ON_CALL(*mock_process_thread_manager, get_mpi_size())
        .WillByDefault(Return(mpi_size));

    ON_CALL(*mock_process_thread_manager, get_mpi_comm())
        .WillByDefault(Return(MPI_COMM_WORLD));

    ON_CALL(*mock_process_thread_manager, get_openmp_size())
        .WillByDefault(Return(4));

    Processing_info *processing_info;
    Processing_unit **processing_units;
    int num_total_processing_units;
    unsigned int hostname_checksum;
    int common_id_prev;

    processing_info = new Processing_info();
    //processing_info->pick_out_actived_processing_units(1,1, 200.0);
    processing_units = processing_info->get_processing_units();
    num_total_processing_units = processing_info->get_num_total_processing_units();

    ASSERT_GT(num_total_processing_units, 0);
    ASSERT_LE(num_total_processing_units, mpi_size * 4);
    ASSERT_NE(processing_units, (void*)NULL);

    hostname_checksum = processing_units[0]->hostname_checksum;
    common_id_prev = 0;
    for(int i = 0; i < num_total_processing_units; i ++) {
        ASSERT_EQ(processing_units[i]->hostname_checksum, hostname_checksum);
        ASSERT_GE(processing_units[i]->process_id, 0);
        ASSERT_LT(processing_units[i]->process_id, mpi_size);
        ASSERT_GE(processing_units[i]->thread_id, 0);
        ASSERT_LT(processing_units[i]->thread_id, 4);
        if(i != num_total_processing_units - 1)
            ASSERT_EQ(processing_units[i]->common_id, processing_units[i+1]->common_id - 1);
    }

    delete grid_info_mgr;
    delete process_thread_mgr;
};

TEST(ProcessingUnitsInfoTest, DifferentHostname) {
    NiceMock<Mock_Process_thread_manager> *mock_process_thread_manager = new NiceMock<Mock_Process_thread_manager>;
    process_thread_mgr = mock_process_thread_manager;
    grid_info_mgr = new Grid_info_manager;

    char *hostname;
    int len;
    ON_CALL(*mock_process_thread_manager, get_hostname(_, _))
        .WillByDefault(Invoke(get_different_hostname));

    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    ON_CALL(*mock_process_thread_manager, get_mpi_rank())
        .WillByDefault(Return(mpi_rank));

    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    ON_CALL(*mock_process_thread_manager, get_mpi_size())
        .WillByDefault(Return(mpi_size));

    ON_CALL(*mock_process_thread_manager, get_mpi_comm())
        .WillByDefault(Return(MPI_COMM_WORLD));

    ON_CALL(*mock_process_thread_manager, get_openmp_size())
        .WillByDefault(Return(4));

    Processing_info *processing_info;
    Processing_unit **processing_units;
    int num_total_processing_units;
    unsigned int hostname_checksum;
    int common_id_prev;

    processing_info = new Processing_info();
    //processing_info->pick_out_actived_processing_units(1,1, 200.0);
    processing_units = processing_info->get_processing_units();
    num_total_processing_units = processing_info->get_num_total_processing_units();

    ASSERT_GT(num_total_processing_units, 0);
    ASSERT_LE(num_total_processing_units, mpi_size * 4);
    ASSERT_NE(processing_units, (void*)NULL);

    unsigned int checksums[128];
    int num_checksums = 0;
    bool have_this_checksum;
    common_id_prev = 0;
    for(int i = 0; i < num_total_processing_units; i ++) {
        have_this_checksum = false;
        for(int j = 0; j < num_checksums; j++)
            if(checksums[j] == processing_units[i]->hostname_checksum)
                have_this_checksum = true;
        if(!have_this_checksum)
            checksums[num_checksums++] = processing_units[i]->hostname_checksum;
        ASSERT_LE(num_checksums, 5);
        ASSERT_GE(processing_units[i]->process_id, 0);
        ASSERT_LT(processing_units[i]->process_id, mpi_size);
        ASSERT_GE(processing_units[i]->thread_id, 0);
        ASSERT_LT(processing_units[i]->thread_id, 4);
        if(i != num_total_processing_units - 1)
            ASSERT_EQ(processing_units[i]->common_id, processing_units[i+1]->common_id - 1);
    }

    delete grid_info_mgr;
    delete process_thread_mgr;
};
