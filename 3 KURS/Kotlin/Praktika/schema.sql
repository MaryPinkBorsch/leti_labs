
    create table brand (
        rating integer,
        since integer,
        id bigint not null auto_increment,
        name varchar(255) not null,
        primary key (id)
    ) engine=InnoDB;

    create table category (
        id bigint not null auto_increment,
        category_name varchar(255) not null,
        primary key (id)
    ) engine=InnoDB;

    create table clothing (
        price integer,
        season integer,
        brand_id bigint,
        category_id bigint,
        id bigint not null auto_increment,
        model_name varchar(255) not null,
        primary key (id)
    ) engine=InnoDB;

    create table order_clothing (
        clothing_id bigint not null,
        order_id bigint not null
    ) engine=InnoDB;

    create table orders (
        clothing_id bigint not null,
        id bigint not null auto_increment,
        order_date datetime(6) not null,
        user_id bigint not null,
        status enum ('ACTIVE','FINISHED','PAID'),
        primary key (id)
    ) engine=InnoDB;

    create table users (
        created_at datetime(6),
        id bigint not null auto_increment,
        email varchar(255) not null,
        first_name varchar(255) not null,
        last_name varchar(255) not null,
        password varchar(255) not null,
        primary key (id)
    ) engine=InnoDB;

    alter table brand 
       add constraint UKrdxh7tq2xs66r485cc8dkxt77 unique (name);

    alter table category 
       add constraint UKlroeo5fvfdeg4hpicn4lw7x9b unique (category_name);

    alter table users 
       add constraint UK6dotkott2kjsp8vw4d0m25fb7 unique (email);

    alter table clothing 
       add constraint FKmwf6v6oaxc6qqsiiqnkjaahng 
       foreign key (brand_id) 
       references brand (id);

    alter table clothing 
       add constraint FK835w5bt7bea95pdw3xquet8h9 
       foreign key (category_id) 
       references category (id);

    alter table order_clothing 
       add constraint FKewmt5eodm6yst3nqd25gcvtvx 
       foreign key (clothing_id) 
       references clothing (id);

    alter table order_clothing 
       add constraint FKil668hdw5x5apaixyhj9l56su 
       foreign key (order_id) 
       references orders (id);

    alter table orders 
       add constraint FKr1a7odehhaelogr2dfkjv7qh2 
       foreign key (clothing_id) 
       references clothing (id);

    alter table orders 
       add constraint FK32ql8ubntj5uh44ph9659tiih 
       foreign key (user_id) 
       references users (id);
