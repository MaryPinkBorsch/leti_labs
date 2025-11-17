// ClothingRepository.java
package com.example.masya.sportstore.repository;

import com.example.masya.sportstore.entity.Clothing;
import com.example.masya.sportstore.entity.Brand;
import com.example.masya.sportstore.entity.Category;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface ClothingRepository2 extends JpaRepository<Clothing, Long> {
    List<Clothing> findByBrand(Brand brand);
    List<Clothing> findByCategory(Category category);
    List<Clothing> findByBrandAndCategory(Brand brand, Category category);
    List<Clothing> findByPriceBetween(Integer minPrice, Integer maxPrice);
    List<Clothing> findByModelNameContainingIgnoreCase(String modelName);
    List<Clothing> findBySeason(Integer season);
    
    @Query("SELECT c FROM Clothing c WHERE c.brand.id = :brandId ORDER BY c.price DESC")
    List<Clothing> findTopRatedByBrandId(@Param("brandId") Long brandId, @Param("limit") int limit);
    
    List<Clothing> findByCategoryCategoryName(String categoryName);
    List<Clothing> findByBrandName(String brandName);
    
    @Query("SELECT c FROM Clothing c WHERE LOWER(c.modelName) LIKE LOWER(:searchTerm) OR LOWER(c.brand.name) LIKE LOWER(:searchTerm) OR LOWER(c.category.categoryName) LIKE LOWER(:searchTerm)")
    List<Clothing> searchByModelNameOrBrandOrCategory(@Param("searchTerm") String searchTerm);
    
    @Query("SELECT c FROM Clothing c WHERE " +
           "(:modelName IS NULL OR LOWER(c.modelName) LIKE LOWER(:modelName)) AND " +
           "(:brandName IS NULL OR LOWER(c.brand.name) = LOWER(:brandName)) AND " +
           "(:categoryName IS NULL OR LOWER(c.category.categoryName) = LOWER(:categoryName)) AND " +
           "(:minPrice IS NULL OR c.price >= :minPrice) AND " +
           "(:maxPrice IS NULL OR c.price <= :maxPrice) AND " +
           "(:season IS NULL OR c.season = :season)")
    List<Clothing> advancedSearch(@Param("modelName") String modelName,
                                 @Param("brandName") String brandName,
                                 @Param("categoryName") String categoryName,
                                 @Param("minPrice") Integer minPrice,
                                 @Param("maxPrice") Integer maxPrice,
                                 @Param("season") Integer season);
    
    @Query("SELECT c FROM Clothing c WHERE c.category.id = :categoryId AND c.brand.id = :brandId AND c.id != :excludeId ORDER BY c.price DESC LIMIT :limit")
    List<Clothing> findSimilarItems(@Param("categoryId") Long categoryId,
                                   @Param("brandId") Long brandId,
                                   @Param("excludeId") Long excludeId,
                                   @Param("limit") int limit);
    
    @Query(value = "SELECT c.* FROM clothing c ORDER BY c.price DESC LIMIT :limit", nativeQuery = true)
    List<Clothing> findPopularItems(@Param("limit") int limit);
    
    @Query(value = "SELECT c.* FROM clothing c ORDER BY c.id DESC LIMIT :limit", nativeQuery = true)
    List<Clothing> findNewArrivals(@Param("limit") int limit);
}