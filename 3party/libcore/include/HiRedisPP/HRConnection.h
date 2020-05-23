
#ifndef _HRCONNECTION_H_
#define _HRCONNECTION_H_

#include <string>
#include <vector>

#include "Config.h"
#include "Timestamp.h"
#include "hiredis/hiredis.h"

// MIGRATE时不删除本地数据
#define REDIS_COPY 0x0001
// MIGRATE替换目标数据
#define REDIS_REPLACE 0x0002

enum EBitOp
{
    EBOP_AND = 0,
    EBOP_OR,
    EBOP_XOR,
    EBOP_NOT,
};

enum EPivot
{
    EP_BEFORE = 0,
    EP_AFTER
};

enum EObjectCmd
{
    EOC_REFCOUNT = 0,
    EOC_ENCODING,
    EOC_IDLETIME,
};

namespace XSpace
{

class HRConnection
{
public:
    explicit HRConnection(bool autoreconnect = true, bool waitconnected = true);
    ~HRConnection();

    bool Connect(const char* ip, int port, int db = 0, const char* password = NULL);
    bool IsValid() const { return _context != NULL; };

    // BEGIN CONNECTION
    bool Auth(const char* password = NULL);
    bool Ping();
    bool SelectDB(int db);
    bool Quit();
    void Close();
    void Shutdown();
    bool OnConnected();
    bool TryReconnect();
    // END CONNECTION

    // BEGIN KEY
    int Del(const char* key);
    int Del(const std::vector<const char*>& keys);
    bool Dump(const char* key, _OUT std::string& value);
    std::string Dump(const char* key);
    int Exists(const char* key);
    int Expire(const char* key, int seconds);
    int ExpireAT(const char* key, const Timestamp& expired);
    size_t Keys(const char* fmt, _OUT std::vector<std::string>& keys);
    // timeout/ms, flag=COPY|REPLACE
    bool Migrate(const char* dstip, int dstport, int dstdb, const char* key, int timeout, int flag);
    bool Move(int dstdb, const char* key);
    long long ObjectRefCount(const char* key);
    long long ObjectIdleTime(const char* key);
    std::string ObjectEncoding(const char* key);
    int Persist(const char* key);
    int PExpire(const char* key, int ms);
    int PExpireAT(const char* key, const Timestamp& expired);
    long PTTL(const char* key);
    bool RandomKey(_OUT std::string& data);
    std::string RandomKey();
    bool Rename(const char* key, const char* newkey);
    bool RenameNX(const char* key, const char* newkey);
    // tts/ms
    bool Restore(const char* key, const char* value, size_t length, int ttl = 0, bool replace = false);
    bool Restore(const char* key, const std::string& value, int ttl = 0, bool replace = false);
    // SORT key [BY pattern] [LIMIT offset count] [GET pattern [GET pattern ...]] [ASC | DESC] [ALPHA] [STORE destination]
    bool Sort(const char* key, _OUT std::vector<std::string>& value);
    long TTL(const char* key);
    bool Type(const char* key, _OUT std::string& data);
    std::string Type(const char* key);
    // END KEY

    // BEGIN STRING
    int Append(const char* key, const char* value);
    int BitCount(const char* key);
    int BitCount(const char* key, int start, int end);
    int BitOp(EBitOp op, const char* dstkey, const std::vector<const char*>& keys);
    long long Decr(const char* key);
    inline long long Decr(const char* key, long long decr) { return DecrBy(key, decr); }
    long long DecrBy(const char* key, long long decr);
    bool Get(const char* key, _OUT std::string& value);
    std::string Get(const char* key);
    int GetBit(const char* key, int offset);
    bool GetRange(const char* key, int start, int end, _OUT std::string& value);
    std::string GetRange(const char* key, int start, int end);
    bool GetSet(const char* key, const char* newvalue, _OUT std::string& oldvalue);
    std::string GetSet(const char* key, const char* newvalue);
    long long Incr(const char* key);
    long long Incr(const char* key, long long incr);
    double Incr(const char* key, double incr);
    size_t MGet(const std::vector<const char*>& keys, _OUT std::vector<std::string>& values);
    bool MSet(const std::vector<const char*>& keys, const std::vector<const char*>& values);
    bool MSetNX(const std::vector<const char*>& keys, const std::vector<const char*>& values);
    bool PSetEx(const char* key, const char* value, long long expire_ms);
    bool Set(const char* key, const char* value);
    int SetBit(const char* key, int offset, bool value);
    bool SetEx(const char* key, const char* value, long long expire_s);
    int SetNX(const char* key, const char* value);
    int SetRange(const char* key, const char* value, int offset);
    int Strlen(const char* key);
    // END STRING

    // BEGIN HASH
    int HDel(const char* key, const char* field);
    int HDel(const char* key, const char* field, ...);
    int HDel(const char* key, const std::vector<const char*>& fields);
    int HExists(const char* key, const char* field);
    bool HGet(const char* key, const char* field, _OUT std::string& value);
    std::string HGet(const char* key, const char* field);
    size_t HGetAll(const char* key, _OUT std::vector<std::string>& fieldvalues);
    long long HIncr(const char* key, const char* field, int incr) { return HIncrBy(key, field, (long long)incr); }
    long long HIncrBy(const char* key, const char* field, int incr) { return HIncrBy(key, field, (long long)incr); }
    inline long long HIncr(const char* key, const char* field, long long incr) { return HIncrBy(key, field, (long long)incr); }
    long long HIncrBy(const char* key, const char* field, long long incr);
    double HIncrBy(const char* key, const char* field, double incr);
    inline double HIncrByFloat(const char* key, const char* field, double incr) { return HIncrBy(key, field, incr); }
    size_t HKeys(const char* key, _OUT std::vector<std::string>& keys);
    size_t HLen(const char* key);
    size_t HMGet(const char* key, const std::vector<const char*>& fields, _OUT std::vector<std::string>& values);
    bool HMSet(const char* key, const std::vector<const char*>& fields, const std::vector<const char*>& values);
    // result: 1 - new 0 - replace < 0 - failed
    int HSet(const char* key, const char* field, const char* value);
    int HSetNX(const char* key, const char* field, const char* value);
    size_t HVals(const char* key, _OUT std::vector<std::string>& values);
    size_t HScan(const char* key, _OUT std::vector<std::string>& values);
    // END HASH

    // BEGIN LIST
    // blocked when list is empty, timeout s
    bool BLPop(const char* key, _OUT std::string& value, int timeout = 0 /*s*/);
    std::string BLPop(const char* key, int timeout = 0 /*s*/);
    bool BLPop(const std::vector<const char*>& keys, _OUT std::vector<std::string>& rkeys, _OUT std::vector<std::string>& values, int timeout = 0 /*s*/);
    bool BRPop(const char* key, _OUT std::string& value, int timeout = 0);
    std::string BRPop(const char* key, int timeout = 0 /*s*/);
    bool BRPop(const std::vector<const char*>& keys, _OUT std::vector<std::string>& rkeys, _OUT std::vector<std::string>& values, int timeout);
    bool BRPopLPush(const char* src, const char* dst, _OUT std::string& poped, int timeout = 0);
    std::string BRPopLPush(const char* src, const char* dst, int timeout = 0);
    bool LIndex(const char* key, int idx, _OUT std::string& value);
    std::string LIndex(const char* key, int idx);
    // return the len of the list
    long long LInsert(const char* key, EPivot pivot, const char* pivotvalue, const char* value);
    long long LLen(const char* key);
    bool LPop(const char* key, _OUT std::string& value);
    std::string LPop(const char* key);
    // result: the len of the list
    int LPush(const char* key, const char* value);
    int LPush(const char* key, const std::vector<const char*>& values);
    int LPushX(const char* key, const char* value);
    size_t LRange(const char* key, int start, int stop, _OUT std::vector<std::string>& values);
    /*
    count > 0 : 从表头开始向表尾搜索，移除与 value 相等的元素，数量为 count 。
    count < 0 : 从表尾开始向表头搜索，移除与 value 相等的元素，数量为 count 的绝对值。
    count = 0 : 移除表中所有与 value 相等的值。
    */
    int LRem(const char* key, const char* value, int count = 0);
    bool LSet(const char* key, int idx, const char* value);
    bool LTrim(const char* key, int start, int stop);
    bool RPop(const char* key, _OUT std::string& value);
    std::string RPop(const char* key);
    bool RPopLPush(const char* src, const char* dst, std::string& popped);
    std::string RPopLPush(const char* src, const char* dst);
    int RPush(const char* key, const char* value);
    int RPush(const char* key, const std::vector<const char*>& values);
    int RPushX(const char* key, const char* value);
    // END LIST

    // BEGIN SET
    int SAdd(const char* key, const char* value);
    int SAdd(const char* key, const std::vector<const char*>& values);
    int SCard(const char* key);
    size_t SDiff(const char* key, std::vector<std::string>& values);
    size_t SDiff(const std::vector<const char*>& keys, std::vector<std::string>& values);
    int SDiffStore(const char* dst, const char* key);
    int SDiffStore(const char* dst, const std::vector<const char*>& keys);
    size_t SInter(const char* key, std::vector<std::string>& values);
    size_t SInter(const std::vector<const char*>& keys, std::vector<std::string>& values);
    int SInterStore(const char* dst, const char* key);
    int SInterStore(const char* dst, const std::vector<const char*>& keys);
    int SIsMember(const char* key, const char* member);
    size_t SMembers(const char* key, _OUT std::vector<std::string>& members);
    int SMove(const char* src, const char* dst, const char* member);
    bool SPop(const char* key, _OUT std::string& value);
    std::string SPop(const char* key);
    bool SRandMember(const char* key, _OUT std::string& value);
    std::string SRandMember(const char* key);
    int SRem(const char* key, const char* member);
    int SRem(const char* key, const std::vector<const char*>& members);
    size_t SUnion(const char* key, std::vector<std::string>& values);
    size_t SUnion(const std::vector<const char*>& keys, std::vector<std::string>& values);
    int SUnionStore(const char* dst, const char* key);
    int SUnionStore(const char* dst, const std::vector<const char*>& keys);
    // END SET

    // BEGIN SORTEDSET
    int ZAdd(const char* key, long long score, const char* value);
    inline int ZAdd(const char* key, int score, const char* value) { return ZAdd(key, (long long)score, value); }
    int ZAdd(const char* key, double score, const char* value);
    int ZAdd(const char* key, const std::vector<long long>& scores, const std::vector<const char*>& values);
    int ZAdd(const char* key, const std::vector<double>& scores, const std::vector<const char*>& values);
    int ZAdd(const char* key, const std::vector<const char*>& scores, const std::vector<const char*>& values);
    bool ZRange(const char* key, int start, int stop, std::vector<std::string>& values, bool withscore = false);
    // no support for limit
    // min/max -inf,+inf,number,(number,number), ( means >, ) means <
    // ZRangeByScore("zset", "(3", "88)", values, true); >3 and < 88
    bool ZRangeByScore(const char* key, const char* min, const char* max, std::vector<std::string>& values, bool withscore = false);
    long long ZRank(const char* key, const char* member);
    int ZRem(const char* key, const char* member);
    int ZRem(const char* key, const std::vector<const char*>& members);
    int ZRemRangeByRank(const char* key, int start, int stop);
    inline int ZRemRangeByScore(const char* key, int min, int max) { return ZRemRangeByScore(key, (long long)min, (long long)max); }
    int ZRemRangeByScore(const char* key, long long min, long long max);
    int ZRemRangeByScore(const char* key, double min, double max);
    bool ZRevRange(const char* key, int start, int stop, std::vector<std::string>& values, bool withscore = false);
    bool ZRevRangeByScore(const char* key, const char* max, const char* min, std::vector<std::string>& values, bool withscore = false);
    long long ZRevRank(const char* key, const char* member);
    bool ZScore(const char* key, const char* member, long long& score);
    bool ZScore(const char* key, const char* member, double& score);
    // ZUNIONSTORE TODO:
    // ZINTERSTORE TODO:
    // ZSCAN TODO:
    // END SORTEDSET

    // BEGIN STREAM
    // END STREAM

    // BEGIN PUB/SUB
    // END PUB/SUB

    // BEGIN TRANSACTION
    bool Discard();
    bool Exec();
    bool Multi();
    bool UnWatch();
    bool Watch(const char* key);
    bool Watch(const std::vector<const char*>& key);
    // END TRANSACTION

    // BEGIN SCRIPT
    //EVAL
    //EVALSHA
    //SCRIPT EXISTS
    //SCRIPT FLUSH
    //SCRIPT KILL
    //SCRIPT LOAD
    // END SCRIPT

    // BEGIN SERVER
    // END SERVER

private:
    redisContext* _context;
    std::string _ip;
    int _port;
    int _db;
    char _password[512];
    bool _connected;
    // 是否自动重连接
    bool _autoreconnect;
    // 如果断线，是否等待连接成功后才返回
    bool _waitconnected;
};

} // namespace XSpace

#endif // _HRCONNECTION_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
