package com.example.masya.sportstore.service;

import com.example.masya.sportstore.entity.Clothing;
import java.util.List;

public interface SearchService {
    List<Clothing> searchClothing(String query);
    List<Clothing> advancedSearch(String modelName, String brandName, String categoryName, 
                                 Integer minPrice, Integer maxPrice, Integer season);
    List<Clothing> searchByFilters(Map<String, Object> filters);
    List<Clothing> findSimilarItems(Long clothingId, int limit);
    List<Clothing> findPopularItems(int limit);
    List<Clothing> findNewArrivals(int limit);
    List<Clothing> findSeasonalItems(Integer season);
}