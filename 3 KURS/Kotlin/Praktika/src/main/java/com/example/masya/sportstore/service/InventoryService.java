package com.example.masya.sportstore.service;

import com.example.masya.sportstore.entity.Clothing;
import java.util.List;
import java.util.Map;

public interface InventoryService {
    boolean isInStock(Long clothingId);
    Integer getStockQuantity(Long clothingId);
    void updateStock(Long clothingId, Integer quantity);
    void decreaseStock(Long clothingId, Integer quantity);
    void increaseStock(Long clothingId, Integer quantity);
    List<Clothing> getLowStockItems(Integer threshold);
    List<Clothing> getOutOfStockItems();
    Map<Long, Integer> getBulkStockInfo(List<Long> clothingIds);
    void restockItem(Long clothingId, Integer quantity);
}