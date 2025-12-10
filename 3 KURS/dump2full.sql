-- MySQL dump 10.13  Distrib 8.0.44, for Win64 (x86_64)
--
-- Host: 127.0.0.1    Database: sportstore_db
-- ------------------------------------------------------
-- Server version	8.0.44-0ubuntu0.24.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Temporary view structure for view `active_orders_detailed`
--

DROP TABLE IF EXISTS `active_orders_detailed`;
/*!50001 DROP VIEW IF EXISTS `active_orders_detailed`*/;
SET @saved_cs_client     = @@character_set_client;
/*!50503 SET character_set_client = utf8mb4 */;
/*!50001 CREATE VIEW `active_orders_detailed` AS SELECT 
 1 AS `order_id`,
 1 AS `order_date`,
 1 AS `user_id`,
 1 AS `first_name`,
 1 AS `last_name`,
 1 AS `full_name`,
 1 AS `email`,
 1 AS `total_items`,
 1 AS `total_price`,
 1 AS `items_list`,
 1 AS `formatted_date`,
 1 AS `hours_since_order`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `brand`
--

DROP TABLE IF EXISTS `brand`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `brand` (
  `rating` int DEFAULT NULL,
  `since` int DEFAULT NULL,
  `id` bigint NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `UKrdxh7tq2xs66r485cc8dkxt77` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `brand`
--

LOCK TABLES `brand` WRITE;
/*!40000 ALTER TABLE `brand` DISABLE KEYS */;
INSERT INTO `brand` VALUES (5,1964,1,'Nike'),(2,1949,2,'Adidas'),(4,1948,3,'Puma'),(3,2005,4,'Sportik'),(5,1999,5,'Demix'),(5,2000,6,'Pinkxx'),(4,1980,7,'Torres'),(4,1981,8,'MotionOne'),(5,2010,9,'LazyFitness'),(1,2021,10,'Abibas');
/*!40000 ALTER TABLE `brand` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `category`
--

DROP TABLE IF EXISTS `category`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `category` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `category_name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `UKlroeo5fvfdeg4hpicn4lw7x9b` (`category_name`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `category`
--

LOCK TABLES `category` WRITE;
/*!40000 ALTER TABLE `category` DISABLE KEYS */;
INSERT INTO `category` VALUES (5,'Ballet dresses'),(2,'Basketball Apparel'),(3,'Football Jerseys'),(4,'Gymnastic clothes'),(6,'Iceskating shoes'),(1,'Running Shoes'),(7,'Swimming suits');
/*!40000 ALTER TABLE `category` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `clothing`
--

DROP TABLE IF EXISTS `clothing`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `clothing` (
  `price` int DEFAULT NULL,
  `season` int DEFAULT NULL,
  `brand_id` bigint DEFAULT NULL,
  `category_id` bigint DEFAULT NULL,
  `id` bigint NOT NULL AUTO_INCREMENT,
  `model_name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FKmwf6v6oaxc6qqsiiqnkjaahng` (`brand_id`),
  KEY `FK835w5bt7bea95pdw3xquet8h9` (`category_id`),
  CONSTRAINT `FK835w5bt7bea95pdw3xquet8h9` FOREIGN KEY (`category_id`) REFERENCES `category` (`id`),
  CONSTRAINT `FKmwf6v6oaxc6qqsiiqnkjaahng` FOREIGN KEY (`brand_id`) REFERENCES `brand` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `clothing`
--

LOCK TABLES `clothing` WRITE;
/*!40000 ALTER TABLE `clothing` DISABLE KEYS */;
INSERT INTO `clothing` VALUES (150,2,3,1,1,'Air Max 270'),(180,2,2,1,2,'Ultraboost 22'),(80,2,1,2,3,'Basketball Jersey'),(666,2,6,4,4,'Gymnastic dancing suit'),(180,2,4,5,5,'Ballet pink tutu'),(50,2,5,6,6,'Iceskating white shoes'),(144,2,7,7,7,'Violet Swimming sportsuit'),(800,2,8,4,8,'Red Warmup suit'),(605,2,9,5,9,'Pink Ballet pointyshoes'),(25,2,10,2,10,'Basketball blue suit');
/*!40000 ALTER TABLE `clothing` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `order_clothing`
--

DROP TABLE IF EXISTS `order_clothing`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `order_clothing` (
  `clothing_id` bigint NOT NULL,
  `order_id` bigint NOT NULL,
  KEY `FKil668hdw5x5apaixyhj9l56su` (`order_id`),
  KEY `FKewmt5eodm6yst3nqd25gcvtvx` (`clothing_id`),
  CONSTRAINT `FKewmt5eodm6yst3nqd25gcvtvx` FOREIGN KEY (`clothing_id`) REFERENCES `clothing` (`id`),
  CONSTRAINT `FKil668hdw5x5apaixyhj9l56su` FOREIGN KEY (`order_id`) REFERENCES `orders` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `order_clothing`
--

LOCK TABLES `order_clothing` WRITE;
/*!40000 ALTER TABLE `order_clothing` DISABLE KEYS */;
INSERT INTO `order_clothing` VALUES (1,2),(2,2),(3,2),(4,2),(1,3),(2,3),(3,3),(4,3);
/*!40000 ALTER TABLE `order_clothing` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`masha`@`localhost`*/ /*!50003 TRIGGER `trg_update_order_total` AFTER INSERT ON `order_clothing` FOR EACH ROW BEGIN
    -- Обновляем общую сумму заказа
    UPDATE orders o
    SET order_total = (
        SELECT IFNULL(SUM(c.price), 0)
        FROM order_clothing oc
        JOIN clothing c ON oc.clothing_id = c.id
        WHERE oc.order_id = NEW.order_id
    )
    WHERE o.id = NEW.order_id;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `orders`
--

DROP TABLE IF EXISTS `orders`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `orders` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `order_date` datetime(6) NOT NULL,
  `user_id` bigint NOT NULL,
  `status` enum('ACTIVE','FINISHED','PAID') DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK32ql8ubntj5uh44ph9659tiih` (`user_id`),
  CONSTRAINT `FK32ql8ubntj5uh44ph9659tiih` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `orders`
--

LOCK TABLES `orders` WRITE;
/*!40000 ALTER TABLE `orders` DISABLE KEYS */;
INSERT INTO `orders` VALUES (2,'2025-12-10 21:58:52.000000',1,'PAID'),(3,'2025-12-10 21:59:27.000000',1,'ACTIVE');
/*!40000 ALTER TABLE `orders` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`masha`@`localhost`*/ /*!50003 TRIGGER `trg_validate_order_before_insert` BEFORE INSERT ON `orders` FOR EACH ROW BEGIN
    -- Устанавливаем дату заказа, если не указана
    IF NEW.order_date IS NULL THEN
        SET NEW.order_date = NOW();
    END IF;
    
    -- Устанавливаем статус по умолчанию
    IF NEW.status IS NULL THEN
        SET NEW.status = 'ACTIVE';
    END IF;
    
    -- Проверяем существование пользователя
    IF NOT EXISTS (SELECT 1 FROM users WHERE id = NEW.user_id) THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Пользователь не существует';
    END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`masha`@`localhost`*/ /*!50003 TRIGGER `trg_log_order_status_change` AFTER UPDATE ON `orders` FOR EACH ROW BEGIN
    IF OLD.status != NEW.status THEN
        -- Логируем изменение статуса
        INSERT INTO order_status_log (order_id, old_status, new_status, changed_at)
        VALUES (NEW.id, OLD.status, NEW.status, NOW());
        
        -- Если заказ оплачен, можно выполнить дополнительные действия
        IF NEW.status = 'PAID' THEN
            -- Например, обновить статистику пользователя
            UPDATE users 
            SET last_purchase_date = NOW(),
                total_purchases = IFNULL(total_purchases, 0) + 1
            WHERE id = NEW.user_id;
        END IF;
    END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Temporary view structure for view `paid_orders_summary`
--

DROP TABLE IF EXISTS `paid_orders_summary`;
/*!50001 DROP VIEW IF EXISTS `paid_orders_summary`*/;
SET @saved_cs_client     = @@character_set_client;
/*!50503 SET character_set_client = utf8mb4 */;
/*!50001 CREATE VIEW `paid_orders_summary` AS SELECT 
 1 AS `order_id`,
 1 AS `order_date`,
 1 AS `user_id`,
 1 AS `first_name`,
 1 AS `last_name`,
 1 AS `customer_name`,
 1 AS `email`,
 1 AS `items_count`,
 1 AS `order_total`,
 1 AS `order_date_only`,
 1 AS `formatted_date`,
 1 AS `order_time`,
 1 AS `items_list`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `users` (
  `created_at` datetime(6) DEFAULT NULL,
  `id` bigint NOT NULL AUTO_INCREMENT,
  `email` varchar(255) NOT NULL,
  `first_name` varchar(255) NOT NULL,
  `last_name` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `UK6dotkott2kjsp8vw4d0m25fb7` (`email`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES (NULL,1,'john@email.com','John','Doe','password123'),(NULL,2,'jane@email.com','Jane','Smith','password123'),(NULL,3,'mary@email.com','Mary','Pink','password123'),(NULL,4,'sveta@email.com','Sveta','Apple','password123'),(NULL,5,'julia@email.com','Julia','Blue','password123'),(NULL,6,'ilia@email.com','Ilia','Cat','password123');
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping events for database 'sportstore_db'
--

--
-- Dumping routines for database 'sportstore_db'
--
/*!50003 DROP FUNCTION IF EXISTS `GetItemPopularity` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`masha`@`localhost` FUNCTION `GetItemPopularity`(
    p_clothing_id BIGINT
) RETURNS varchar(20) CHARSET utf8mb4
    READS SQL DATA
    DETERMINISTIC
BEGIN
    DECLARE v_order_count INT DEFAULT 0;
    
    SELECT COUNT(DISTINCT order_id) INTO v_order_count
    FROM order_clothing
    WHERE clothing_id = p_clothing_id;
    
    RETURN CASE 
        WHEN v_order_count = 0 THEN 'Непопулярный'
        WHEN v_order_count <= 5 THEN 'Средне популярный'
        WHEN v_order_count <= 20 THEN 'Популярный'
        ELSE 'Хит продаж'
    END;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `GetOrderTotal` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`masha`@`localhost` FUNCTION `GetOrderTotal`(
    p_order_id BIGINT
) RETURNS decimal(10,2)
    READS SQL DATA
    DETERMINISTIC
BEGIN
    DECLARE v_total DECIMAL(10, 2) DEFAULT 0.00;
    DECLARE v_order_exists INT DEFAULT 0;
    
    -- Проверяем существование заказа
    SELECT COUNT(*) INTO v_order_exists
    FROM orders
    WHERE id = p_order_id;
    
    IF v_order_exists = 0 THEN
        RETURN -1;  -- Возвращаем -1 если заказ не найден
    END IF;
    
    -- Считаем сумму
    SELECT IFNULL(SUM(c.price), 0.00) INTO v_total
    FROM order_clothing oc
    JOIN clothing c ON oc.clothing_id = c.id
    WHERE oc.order_id = p_order_id;
    
    RETURN v_total;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `GetUserAverageCheck` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`masha`@`localhost` FUNCTION `GetUserAverageCheck`(
    p_user_id BIGINT
) RETURNS decimal(10,2)
    READS SQL DATA
    DETERMINISTIC
BEGIN
    DECLARE v_avg_check DECIMAL(10, 2) DEFAULT 0.00;
    
    SELECT IFNULL(AVG(order_totals.total), 0.00) INTO v_avg_check
    FROM (
        SELECT o.id, SUM(c.price) as total
        FROM orders o
        JOIN order_clothing oc ON o.id = oc.order_id
        JOIN clothing c ON oc.clothing_id = c.id
        WHERE o.user_id = p_user_id 
        AND o.status = 'PAID'
        GROUP BY o.id
    ) AS order_totals;
    
    RETURN ROUND(v_avg_check, 2);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `AddSingleClothingToOrderShort` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`masha`@`localhost` PROCEDURE `AddSingleClothingToOrderShort`(
    IN p_order_id BIGINT,
    IN p_clothing_id BIGINT
)
BEGIN
    -- Проверка заказа
    IF NOT EXISTS (SELECT 1 FROM orders WHERE id = p_order_id) THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Заказ не найден';
    END IF;
    
    -- Проверка одежды
    IF NOT EXISTS (SELECT 1 FROM clothing WHERE id = p_clothing_id) THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Одежда не найдена';
    END IF;
    
    -- Проверка дубликата
    IF EXISTS (SELECT 1 FROM order_clothing WHERE order_id = p_order_id AND clothing_id = p_clothing_id) THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Товар уже в заказе';
    END IF;
    
    -- Добавление
    INSERT INTO order_clothing VALUES (p_order_id, p_clothing_id);
    
    SELECT 'Товар добавлен' AS result;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `CreateOrderWithClothingSimple` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`masha`@`localhost` PROCEDURE `CreateOrderWithClothingSimple`(
    IN p_user_id BIGINT,
    IN p_clothing_ids TEXT,  -- Список ID одежды через запятую: "1,2,3"
    OUT p_new_order_id BIGINT
)
BEGIN
    DECLARE v_user_exists INT DEFAULT 0;
    DECLARE v_error_message VARCHAR(500);
    
    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        RESIGNAL;
    END;
    
    -- Проверка существования пользователя
    SELECT COUNT(*) INTO v_user_exists 
    FROM users 
    WHERE id = p_user_id;
    
    IF v_user_exists = 0 THEN
        SET v_error_message = 'Ошибка: пользователь не найден.';
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = v_error_message;
    END IF;
    
    -- Проверка, что передан хотя бы один товар
    IF p_clothing_ids IS NULL OR p_clothing_ids = '' THEN
        SET v_error_message = 'Ошибка: список одежды не может быть пустым.';
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = v_error_message;
    END IF;
    
    -- Начало транзакции
    START TRANSACTION;
    
    -- Создание заказа
    INSERT INTO orders (user_id, order_date, status)
    VALUES (p_user_id, NOW(), 'ACTIVE');
    
    -- Получение ID созданного заказа
    SET p_new_order_id = LAST_INSERT_ID();
    
    -- Добавление одежды в заказ через временную таблицу
    CREATE TEMPORARY TABLE IF NOT EXISTS temp_clothing_ids (
        clothing_id BIGINT
    );
    
    -- Очистка временной таблицы
    DELETE FROM temp_clothing_ids;
    
    -- Вставка ID во временную таблицу
    SET @sql = CONCAT('INSERT INTO temp_clothing_ids VALUES (', 
                     REPLACE(p_clothing_ids, ',', '),('), 
                     ')');
    PREPARE stmt FROM @sql;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;
    
    -- Проверка существования всех товаров
    IF EXISTS (
        SELECT 1 FROM temp_clothing_ids t
        LEFT JOIN clothing c ON t.clothing_id = c.id
        WHERE c.id IS NULL
    ) THEN
        ROLLBACK;
        SET v_error_message = 'Ошибка: один или несколько товаров не найдены.';
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = v_error_message;
    END IF;
    
    -- Добавление в order_clothing
    INSERT INTO order_clothing (order_id, clothing_id)
    SELECT p_new_order_id, clothing_id FROM temp_clothing_ids;
    
    -- Получение количества добавленных товаров
    SET @items_count = ROW_COUNT();
    
    -- Фиксация транзакции
    COMMIT;
    
    -- Вывод результата
    SELECT 
        CONCAT(
            'Заказ успешно создан. OrderID = ', 
            p_new_order_id, 
            ', добавлено товаров: ', 
            @items_count
        ) AS message,
        p_new_order_id AS order_id,
        @items_count AS items_count;
    
    -- Очистка временной таблицы
    DROP TEMPORARY TABLE IF EXISTS temp_clothing_ids;
    
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `MarkOrderAsPaid` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`masha`@`localhost` PROCEDURE `MarkOrderAsPaid`(
    IN p_order_id BIGINT
)
BEGIN
    DECLARE v_current_status VARCHAR(20);
    
    -- Проверка существования заказа
    IF NOT EXISTS (SELECT 1 FROM orders WHERE id = p_order_id) THEN
        SIGNAL SQLSTATE '45000' 
        SET MESSAGE_TEXT = 'Заказ не найден';
    END IF;
    
    -- Получаем текущий статус
    SELECT status INTO v_current_status 
    FROM orders 
    WHERE id = p_order_id;
    
    -- Проверяем, можно ли изменить статус
    IF v_current_status = 'PAID' THEN
        SIGNAL SQLSTATE '45000' 
        SET MESSAGE_TEXT = 'Заказ уже оплачен';
    END IF;
    
    -- Меняем статус на PAID
    UPDATE orders 
    SET status = 'PAID' 
    WHERE id = p_order_id;
    
    SELECT 'Статус заказа изменен на "Оплачен"' AS result;
    
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Final view structure for view `active_orders_detailed`
--

/*!50001 DROP VIEW IF EXISTS `active_orders_detailed`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`masha`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `active_orders_detailed` AS select `o`.`id` AS `order_id`,`o`.`order_date` AS `order_date`,`o`.`user_id` AS `user_id`,`u`.`first_name` AS `first_name`,`u`.`last_name` AS `last_name`,concat(`u`.`first_name`,' ',`u`.`last_name`) AS `full_name`,`u`.`email` AS `email`,count(distinct `oc`.`clothing_id`) AS `total_items`,ifnull(sum(`c`.`price`),0) AS `total_price`,group_concat(distinct `c`.`model_name` separator ', ') AS `items_list`,date_format(`o`.`order_date`,'%d.%m.%Y %H:%i') AS `formatted_date`,timestampdiff(HOUR,`o`.`order_date`,now()) AS `hours_since_order` from (((`orders` `o` join `users` `u` on((`o`.`user_id` = `u`.`id`))) left join `order_clothing` `oc` on((`o`.`id` = `oc`.`order_id`))) left join `clothing` `c` on((`oc`.`clothing_id` = `c`.`id`))) where (`o`.`status` = 'ACTIVE') group by `o`.`id`,`o`.`order_date`,`o`.`user_id`,`u`.`first_name`,`u`.`last_name`,`u`.`email` order by `o`.`order_date` desc */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `paid_orders_summary`
--

/*!50001 DROP VIEW IF EXISTS `paid_orders_summary`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`masha`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `paid_orders_summary` AS select `o`.`id` AS `order_id`,`o`.`order_date` AS `order_date`,`o`.`user_id` AS `user_id`,`u`.`first_name` AS `first_name`,`u`.`last_name` AS `last_name`,concat(`u`.`first_name`,' ',`u`.`last_name`) AS `customer_name`,`u`.`email` AS `email`,count(distinct `oc`.`clothing_id`) AS `items_count`,ifnull(sum(`c`.`price`),0) AS `order_total`,cast(`o`.`order_date` as date) AS `order_date_only`,date_format(`o`.`order_date`,'%d.%m.%Y') AS `formatted_date`,date_format(`o`.`order_date`,'%H:%i') AS `order_time`,group_concat(distinct `c`.`model_name` separator ', ') AS `items_list` from (((`orders` `o` join `users` `u` on((`o`.`user_id` = `u`.`id`))) left join `order_clothing` `oc` on((`o`.`id` = `oc`.`order_id`))) left join `clothing` `c` on((`oc`.`clothing_id` = `c`.`id`))) where (`o`.`status` = 'PAID') group by `o`.`id`,`o`.`order_date`,`o`.`user_id`,`u`.`first_name`,`u`.`last_name`,`u`.`email` order by `o`.`order_date` desc */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-12-10 23:10:59
