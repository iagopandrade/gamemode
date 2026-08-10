-- ------------------------------------------------------------------
-- Tabela de empresas do sistema dinamico de company_core
-- ------------------------------------------------------------------

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

    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
