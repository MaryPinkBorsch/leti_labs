package com.example.masya.sportstore.service.impl;

import com.example.masya.sportstore.entity.Clothing;
import com.example.masya.sportstore.repository.ClothingRepository;
import com.example.masya.sportstore.service.SearchService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

@Service
@Transactional
public class SearchServiceImpl implements SearchService {

    private final ClothingRepository clothingRepository;

    @Autowired
    public SearchServiceImpl(ClothingRepository clothingRepository) {
        this.clothingRepository = clothingRepository;
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> searchClothing(String query) {
        if (query == null || query.trim().isEmpty()) {
            return clothingRepository.findAll();
        }
        
        String searchTerm = "%" + query.toLowerCase() + "%";
        return clothingRepository.searchByModelNameOrBrandOrCategory(searchTerm);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> advancedSearch(String modelName, String brandName, String categoryName, 
                                       Integer minPrice, Integer maxPrice, Integer season) {
        return clothingRepository.advancedSearch(
            modelName != null ? "%" + modelName + "%" : null,
            brandName,
            categoryName,
            minPrice,
            maxPrice,
            season
        );
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> searchByFilters(Map<String, Object> filters) {
        String modelName = (String) filters.get("modelName");
        String brandName = (String) filters.get("brandName");
        String categoryName = (String) filters.get("categoryName");
        Integer minPrice = (Integer) filters.get("minPrice");
        Integer maxPrice = (Integer) filters.get("maxPrice");
        Integer season = (Integer) filters.get("season");
        
        return advancedSearch(modelName, brandName, categoryName, minPrice, maxPrice, season);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> findSimilarItems(Long clothingId, int limit) {
        Clothing targetClothing = clothingRepository.findById(clothingId)
            .orElseThrow(() -> new RuntimeException("Clothing not found with id: " + clothingId));
        
        return clothingRepository.findSimilarItems(
            targetClothing.getCategory().getId(),
            targetClothing.getBrand().getId(),
            clothingId,
            limit
        );
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> findPopularItems(int limit) {
        return clothingRepository.findPopularItems(limit);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> findNewArrivals(int limit) {
        return clothingRepository.findNewArrivals(limit);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> findSeasonalItems(Integer season) {
        if (season == null) {
            // Return current season items
            season = getCurrentSeason();
        }
        return clothingRepository.findBySeason(season);
    }

    private Integer getCurrentSeason() {
        // Simple season calculation based on month
        int month = java.time.LocalDate.now().getMonthValue();
        if (month >= 3 && month <= 5) return 1; // Spring
        if (month >= 6 && month <= 8) return 2; // Summer
        if (month >= 9 && month <= 11) return 3; // Autumn
        return 4; // Winter
    }
}