/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-11
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <hikyuu/trade_manage/TradeCostBase.h>
#include "../convert_Datetime.h"
#include "../convert_any.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

class PyTradeCostBase : public TradeCostBase {
    PY_CLONE(PyTradeCostBase, TradeCostBase)

public:
    using TradeCostBase::TradeCostBase;

    CostRecord getBuyCost(const Datetime& datetime, const Stock& stock, price_t price,
                          double num) const override {
        PYBIND11_OVERLOAD_PURE(CostRecord, TradeCostBase, getBuyCost, datetime, stock, price, num);
    }

    CostRecord getSellCost(const Datetime& datetime, const Stock& stock, price_t price,
                           double num) const override {
        PYBIND11_OVERLOAD_PURE(CostRecord, TradeCostBase, getSellCost, datetime, stock, price, num);
    }

    CostRecord getBorrowCashCost(const Datetime& datetime, price_t cash) const override {
        PYBIND11_OVERLOAD(CostRecord, TradeCostBase, getBorrowCashCost, datetime, cash);
    }

    CostRecord getReturnCashCost(const Datetime& borrow_datetime, const Datetime& return_datetime,
                                 price_t cash) const override {
        PYBIND11_OVERLOAD(CostRecord, TradeCostBase, getReturnCashCost, borrow_datetime,
                          return_datetime, cash);
    }

    CostRecord getBorrowStockCost(const Datetime& datetime, const Stock& stock, price_t price,
                                  double num) const override {
        PYBIND11_OVERLOAD(CostRecord, TradeCostBase, getBorrowStockCost, datetime, stock, price,
                          num);
    }

    CostRecord getReturnStockCost(const Datetime& borrow_datetime, const Datetime& return_datetime,
                                  const Stock& stock, price_t price, double num) const override {
        PYBIND11_OVERLOAD(CostRecord, TradeCostBase, getReturnStockCost, borrow_datetime,
                          return_datetime, stock, price, num);
    }
};

void export_TradeCost(py::module& m) {
    py::class_<TradeCostBase, TradeCostPtr, PyTradeCostBase>(m, "TradeCostBase",
                                                             R"(交易成本算法基类

    实现自定义交易成本算法接口：

        :py:meth:`TradeCostBase.getBuyCost` - 【必须】获取买入成本
        :py:meth:`TradeCostBase.getSellCost` - 【必须】获取卖出成本
        :py:meth:`TradeCostBase._clone` - 【必须】子类克隆接口)")

      .def(py::init<const string&>(), R"(
    :param str name: 算法名称)")

      .def_property_readonly("name", &TradeCostBase::name, "交易算法名称")

      .def("__str__", to_py_str<TradeCostBase>)
      .def("__repr__", to_py_str<TradeCostBase>)

      .def("getParam", &TradeCostBase::getParam<boost::any>, R"(getParam(self, name)

    获取指定的参数
    
    :param str name: 参数名称
    :return: 参数值
    :raises out_of_range: 无此参数)")

      .def("setParam", &TradeCostBase::setParam<boost::any>, R"(setParam(self, name, value)
    
    设置参数
        
    :param str name: 参数名称
    :param value: 参数值
    :type value: int | bool | float | string
    :raises logic_error: Unsupported type! 不支持的参数类型)")

      .def("clone", &TradeCostBase::clone, "克隆操作")

      .def("getBuyCost", &TradeCostBase::getBuyCost, py::arg("date"), py::arg("stock"),
           py::arg("price"), py::arg("num"), R"(【重载接口】获取买入成本
        
    :param datetime date: 买入时刻
    :param Stock stock: 买入对象
    :param float price: 买入价格
    :param size_t num: 买入数量
    :return: 交易成本记录
    :rtype: CostRecord)")

      .def("getSellCost", &TradeCostBase::getSellCost, py::arg("date"), py::arg("stock"),
           py::arg("price"), py::arg("num"),
           R"(【重载接口】获取卖出成本
        
    :param datetime date: 卖出时刻
    :param Stock stock: 卖出对象
    :param float price: 卖出价格
    :param int num: 卖出数量
    :return: 交易成本记录
    :rtype: CostRecord)")

      //.def("getBorrowCashCost", &TradeCostBase::getBorrowCashCost, "暂未实现")
      //.def("getReturnCashCost", &TradeCostBase::getReturnCashCost, "暂未实现")
      //.def("getBorrowStockCost", &TradeCostBase::getBorrowStockCost, "暂未实现")
      //.def("getReturnStockCost", &TradeCostBase::getReturnStockCost, "暂未实现")

      DEF_PICKLE(TradeCostPtr);
}