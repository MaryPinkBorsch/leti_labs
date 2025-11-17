package com.example.masya.sportstore.service.impl;

import com.example.masya.sportstore.entity.Clothing;
import com.example.masya.sportstore.repository.ClothingRepository;
import com.example.masya.sportstore.service.InventoryService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

@Service
@Transactional
public class InventoryServiceImpl implements InventoryService {

    // In-memory inventory (in production, use database with inventory table)
    private final Map<Long, Integer> inventory = new ConcurrentHashMap<>();
    
    private final ClothingRepository clothingRepository;

    @Autowired
    public InventoryServiceImpl(ClothingRepository clothingRepository) {
        this.clothingRepository = clothingRepository;
        initializeInventory();
    }

    private void initializeInventory() {
        // Initialize inventory from database or set default values
        List<Clothing> allClothing = clothingRepository.findAll();
        for (Clothing clothing : allClothing) {
            inventory.put(clothing.getId(), 10); // Default stock
        }
    }

    @Override
    public boolean isInStock(Long clothingId) {
        validateClothing(clothingId);
        return inventory.getOrDefault(clothingId, 0) > 0;
    }

    @Override
    public Integer getStockQuantity(Long clothingId) {
        validateClothing(clothingId);
        return inventory.getOrDefault(clothingId, 0);
    }

    @Override
    public void updateStock(Long clothingId, Integer quantity) {
        validateClothing(clothingId);
        if (quantity < 0) {
            throw new IllegalArgumentException("Stock quantity cannot be negative");
        }
        inventory.put(clothingId, quantity);
    }

    @Override
    public void decreaseStock(Long clothingId, Integer quantity) {
        validateClothing(clothingId);
        int currentStock = inventory.getOrDefault(clothingId, 0);
        if (currentStock < quantity) {
            throw new RuntimeException("Insufficient stock for clothing id: " + clothingId);
        }
        inventory.put(clothingId, currentStock - quantity);
    }

    @Override
    public void increaseStock(Long clothingId, Integer quantity) {
        validateClothing(clothingId);
        if (quantity < 0) {
            throw new IllegalArgumentException("Quantity to increase cannot be negative");
        }
        int currentStock = inventory.getOrDefault(clothingId, 0);
        inventory.put(clothingId, currentStock + quantity);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> getLowStockItems(Integer threshold) {
        return inventory.entrySet().stream()
            .filter(entry -> entry.getValue() <= threshold)
            .map(entry -> clothingRepository.findById(entry.getKey()).orElse(null))
            .filter(clothing -> clothing != null)
            .toList();
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> getOutOfStockItems() {
        return getLowStockItems(0);
    }

    @Override
    public Map<Long, Integer> getBulkStockInfo(List<Long> clothingIds) {
        Map<Long, Integer> stockInfo = new HashMap<>();
        for (Long clothingId : clothingIds) {
            if (clothingRepository.existsById(clothingId)) {
                stockInfo.put(clothingId, inventory.getOrDefault(clothingId, 0));
            }
        }
        return stockInfo;
    }

    @Override
    public void restockItem(Long clothingId, Integer quantity) {
        validateClothing(clothingId);
        if (quantity <= 0) {
            throw new IllegalArgumentException("Restock quantity must be positive");
        }
        increaseStock(clothingId, quantity);
    }

    private void validateClothing(Long clothingId) {
        if (!clothingRepository.existsById(clothingId)) {
            throw new RuntimeException("Clothing not found with id: " + clothingId);
        }
    }
}