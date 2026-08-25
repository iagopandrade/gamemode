CREATE TABLE IF NOT EXISTS `companies` (
    `id`              INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `name`            VARCHAR(31)  NOT NULL,

    `locked`          TINYINT(1)   NOT NULL DEFAULT 0,

    `open_hour`       TINYINT UNSIGNED NOT NULL DEFAULT 0,
    `close_hour`      TINYINT UNSIGNED NOT NULL DEFAULT 23,

    `enter_x`         FLOAT NOT NULL DEFAULT 0,
    `enter_y`         FLOAT NOT NULL DEFAULT 0,
    `enter_z`         FLOAT NOT NULL DEFAULT 0,
    `enter_interior`  SMALLINT UNSIGNED NOT NULL DEFAULT 0,

    `exit_x`          FLOAT NOT NULL DEFAULT 0,
    `exit_y`          FLOAT NOT NULL DEFAULT 0,
    `exit_z`          FLOAT NOT NULL DEFAULT 0,
    `exit_interior`   SMALLINT UNSIGNED NOT NULL DEFAULT 0,

    `sales_set`       TINYINT(1) NOT NULL DEFAULT 0,
    `sales_x`         FLOAT NOT NULL DEFAULT 0,
    `sales_y`         FLOAT NOT NULL DEFAULT 0,
    `sales_z`         FLOAT NOT NULL DEFAULT 0,
    `sales_interior`  SMALLINT UNSIGNED NOT NULL DEFAULT 0,
    `sales_size`      FLOAT NOT NULL DEFAULT 3,

    `map_icon_enabled` TINYINT(1) NOT NULL DEFAULT 0,
    `map_icon`         TINYINT UNSIGNED NOT NULL DEFAULT 0,
    `map_icon_color`   INT UNSIGNED NOT NULL DEFAULT 4294967295,

    `pickup_category` TINYINT UNSIGNED NOT NULL DEFAULT 0,

    `owner_id`        INT UNSIGNED NOT NULL DEFAULT 0,

    `cash`             INT UNSIGNED NOT NULL DEFAULT 0,

    PRIMARY KEY (`id`),
    KEY `idx_companies_owner` (`owner_id`),
    KEY `idx_companies_category` (`pickup_category`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `company_pickups` (
    `id`          INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `company_id`  INT UNSIGNED NOT NULL,
    `model`       INT UNSIGNED NOT NULL DEFAULT 1239,
    `x`           FLOAT NOT NULL DEFAULT 0,
    `y`           FLOAT NOT NULL DEFAULT 0,
    `z`           FLOAT NOT NULL DEFAULT 0,
    `interior`    SMALLINT UNSIGNED NOT NULL DEFAULT 0,

    PRIMARY KEY (`id`),
    KEY `idx_company_pickups_company` (`company_id`),
    CONSTRAINT `fk_company_pickups_company`
        FOREIGN KEY (`company_id`) REFERENCES `companies` (`id`)
        ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `company_purchases` (
    `id`          INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `company_id`  INT UNSIGNED NOT NULL,
    `buyer_id`    INT UNSIGNED NOT NULL,
    `item_name`   VARCHAR(32)  NOT NULL,
    `price`       INT UNSIGNED NOT NULL,
    `created_at`  TIMESTAMP    NOT NULL DEFAULT CURRENT_TIMESTAMP,

    PRIMARY KEY (`id`),
    KEY `idx_company_purchases_company` (`company_id`),
    CONSTRAINT `fk_company_purchases_company`
        FOREIGN KEY (`company_id`) REFERENCES `companies` (`id`)
        ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
