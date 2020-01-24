
#ifndef simple_queue_h
#define simple_queue_h

class simple_queue {
public:
    enum queue_ring_e : uint8_t
    {
        NO_RING_QUEUE = 0,
        RING_QUEUE = 1,
    };

    simple_queue( const uint8_t item_size, const uint16_t items_max, const queue_ring_e circular_queue = NO_RING_QUEUE );
    ~simple_queue();

    void     clear();

    uint16_t count();

    int8_t   push( void *data_item );
    int8_t   peek( void *data_item );
    int8_t   pop( void *data_item );

protected:
    typedef struct queue_item_s
    {
        void                *item_data;
        struct queue_item_s *next;
    } _queue_item_t;

    _queue_item_t *_queue_head;
    _queue_item_t *_queue_tail;

    uint8_t       _queue_item_size;
    uint16_t      _queue_max_items;
    queue_ring_e  _queue_circular;
    uint16_t      _queue_items;

    int8_t   _remove_head();
};

simple_queue::simple_queue( const uint8_t item_size, const uint16_t items_max, const queue_ring_e circular_queue )
{
    _queue_max_items = items_max;
    if( _queue_max_items < 1 ) _queue_max_items = 1;

    _queue_item_size = item_size;
    if( _queue_item_size < 1 ) _queue_item_size = 1;

    _queue_circular  = circular_queue;

    clear();
};

simple_queue::~simple_queue()
{
    clear();
};

void simple_queue::clear()
{
    while( _queue_head )
    {
        _remove_head();
    };
};

uint16_t simple_queue::count()
{
    return( _queue_items );
};

int8_t simple_queue::push( void *data_item )
{
    if( _queue_circular == NO_RING_QUEUE && _queue_items == _queue_max_items ) return(0);
    if( _queue_circular == RING_QUEUE    && _queue_items == _queue_max_items ) _remove_head();

    _queue_item_t *item_push = (_queue_item_t *)malloc( sizeof(_queue_item_t) );
    void *tmp_data = (void *)malloc( _queue_item_size );
    memcpy( tmp_data, data_item, _queue_item_size );
    item_push->item_data = tmp_data;
    item_push->next = NULL;
    _queue_items++;

    if( _queue_tail )
    {
        _queue_tail->next = item_push;
        _queue_tail = item_push;
    } else {
        _queue_head = _queue_tail = item_push;
    };

    return(1);
};

int8_t simple_queue::peek( void *data_item )
{
    if( !_queue_head ) return(0);

    memcpy( data_item, _queue_head->item_data, _queue_item_size );

    return(1);
};

int8_t simple_queue::pop( void *data_item )
{
    if( !_queue_head ) return(0);

    memcpy( data_item, _queue_head->item_data, _queue_item_size );
    _remove_head();

    return(1);
};

int8_t simple_queue::_remove_head()
{
    _queue_item_t *item_del = _queue_head;
    if( item_del->next )
    {
        _queue_head = item_del->next;
        _queue_items--;
    } else {
        _queue_head = _queue_tail = NULL;
        _queue_items = 0;
    };
    if( item_del->item_data ) free( item_del->item_data );
    free( item_del );
};

#endif
