typedef enum {
  Mv_Invalid = 0b00,
  Mv_Any     = 0b10,
  Mv_Shared  = 0b11
} xtor_cxl_mv_t;

typedef struct data_mv_t {
  uint32_t data[16] = { 0 };
  xtor_cxl_mv_t mv;
} data_mv_t;

class cxl_mem {

public:
  cxl_mem                ();
  ~cxl_mem               ();

  uint32_t * read                           ( uint64_t addr);
  int write                                 ( uint64_t addr, uint32_t *data);
  int write                                 ( uint64_t addr, uint32_t *data, xtor_cxl_mv_t mv);
  int setMetaValue                          ( uint64_t addr, xtor_cxl_mv_t mv);
  xtor_cxl_mv_t getMetaValue                ( uint64_t addr );

private:


  std::map<uint64_t, data_mv_t> mem_m; // to store tag of line

};


uint32_t * cxl_mem::read ( uint64_t addr)
{
  return mem_m[addr].data;

}

int cxl_mem::write ( uint64_t addr, uint32_t *data)
{
  int rsl = 0;
  std::map<uint64_t, data_mv_t>::iterator it;
  data_mv_t data_mv;

//  data_mv.data = data;

  it = mem_m.find(addr);
  if ( it == mem_m.end() ) {
    memcpy(data_mv.data, data, 16*sizeof(uint32_t));
    mem_m.insert({addr, data_mv});
  }
  else {
    memcpy(it->second.data, data, 16*sizeof(uint32_t));
  }

  return rsl;
}

int cxl_mem::write ( uint64_t addr, uint32_t *data, xtor_cxl_mv_t mv)
{
  int rsl = 0;
  std::map<uint64_t, data_mv_t>::iterator it;
  data_mv_t data_mv;

  memcpy(data_mv.data, data, 16*sizeof(uint32_t));
  data_mv.mv = mv;

  it = mem_m.find(addr);
  if ( it == mem_m.end() )
    mem_m.insert({addr, data_mv});
  else {
    it->second = data_mv;
  }

  return rsl;
}

int cxl_mem::setMetaValue ( uint64_t addr, xtor_cxl_mv_t mv)
{
  int rsl = 0;
  std::map<uint64_t, data_mv_t>::iterator it;
  data_mv_t data_mv;

  data_mv.mv = mv;

  it = mem_m.find(addr);
  if ( it == mem_m.end() )
    mem_m.insert({addr, data_mv});
  else
    it->second.mv = mv;

  return rsl;
}

xtor_cxl_mv_t cxl_mem::getMetaValue( uint64_t addr )
{

  std::map<uint64_t, data_mv_t>::iterator it;
  it = mem_m.find(addr);
  if ( it == mem_m.end() )
    return Mv_Invalid;
  else
    return it->second.mv;

}
