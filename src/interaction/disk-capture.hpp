/*---------------------------------------------------------------------------*\
        .-''''-.         |
       /        \        |
      /_        _\       |  SpaceHub: The Open Source N-body Toolkit
     // \  <>  / \\      |
     |\__\    /__/|      |  Website:  https://yihanwangastro.github.io/SpaceHub/
      \    ||    /       |
        \  __  /         |  Copyright (C) 2019 Yihan Wang
         '.__.'          |
---------------------------------------------------------------------
License
    This file is part of SpaceHub.
    SpaceHub is free software: you can redistribute it and/or modify it under
    the terms of the GPL-3.0 License. SpaceHub is distributed in the hope that it
    will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GPL-3.0 License
    for more details. You should have received a copy of the GPL-3.0 License along
    with SpaceHub.
\*---------------------------------------------------------------------------*/
/**
 * @file diskcapture.hpp
 *
 * Header file.
 */
#pragma once

#include "../dev-tools.hpp"
#include "../spacehub-concepts.hpp"
namespace hub::force {

    const std::vector<double> sg_log_h = {
        -6.382722e-01, -6.401645e-01, -6.439741e-01, -6.458916e-01, -6.497520e-01, -6.516951e-01, -6.556077e-01,
        -6.595559e-01, -6.635403e-01, -6.675615e-01, -6.716204e-01, -6.757175e-01, -6.798537e-01, -6.861328e-01,
        -6.903698e-01, -6.946486e-01, -7.011469e-01, -7.055338e-01, -7.121983e-01, -7.166988e-01, -7.235382e-01,
        -7.281584e-01, -7.351822e-01, -7.399286e-01, -7.471470e-01, -7.520267e-01, -7.594508e-01, -7.644716e-01,
        -7.721133e-01, -7.772835e-01, -7.825161e-01, -7.878124e-01, -7.958800e-01, -8.013429e-01, -8.068754e-01,
        -8.124793e-01, -8.181564e-01, -8.239087e-01, -8.297383e-01, -8.356471e-01, -8.446640e-01, -8.507809e-01,
        -8.569852e-01, -8.632794e-01, -8.696662e-01, -8.794261e-01, -8.860566e-01, -8.927900e-01, -9.030900e-01,
        -9.100949e-01, -9.172146e-01, -9.281180e-01, -9.355420e-01, -9.430951e-01, -9.507820e-01, -9.586073e-01,
        -9.706162e-01, -9.788107e-01, -9.871628e-01, -9.956786e-01, -1.002177e+00, -1.010105e+00, -1.018181e+00,
        -1.026410e+00, -1.034328e+00, -1.041914e+00, -1.050122e+00, -1.057992e+00, -1.065502e+00, -1.073143e+00,
        -1.080922e+00, -1.088842e+00, -1.096367e+00, -1.103474e+00, -1.111259e+00, -1.118615e+00, -1.126098e+00,
        -1.134304e+00, -1.141463e+00, -1.149354e+00, -1.157391e+00, -1.165579e+00, -1.173925e+00, -1.182435e+00,
        -1.191789e+00, -1.200659e+00, -1.209715e+00, -1.219683e+00, -1.229148e+00, -1.238072e+00, -1.247952e+00,
        -1.256490e+00, -1.265200e+00, -1.274088e+00, -1.282329e+00, -1.289037e+00, -1.296709e+00, -1.303644e+00,
        -1.309804e+00, -1.316053e+00, -1.323306e+00, -1.329754e+00, -1.336299e+00, -1.343902e+00, -1.350665e+00,
        -1.358526e+00, -1.366532e+00, -1.374688e+00, -1.381952e+00, -1.389340e+00, -1.396856e+00, -1.403403e+00,
        -1.410050e+00, -1.415669e+00, -1.420216e+00, -1.424812e+00, -1.429457e+00, -1.432974e+00, -1.435334e+00,
        -1.437707e+00, -1.440093e+00, -1.441291e+00, -1.442493e+00, -1.442493e+00, -1.442493e+00, -1.442493e+00,
        -1.442493e+00, -1.441291e+00, -1.441291e+00, -1.440093e+00, -1.438899e+00, -1.437707e+00, -1.437707e+00,
        -1.435334e+00, -1.434152e+00, -1.432974e+00, -1.430626e+00, -1.428291e+00, -1.424812e+00, -1.421361e+00,
        -1.417937e+00, -1.414539e+00, -1.412289e+00, -1.408935e+00, -1.406714e+00, -1.404504e+00, -1.403403e+00,
        -1.403403e+00, -1.404504e+00, -1.405607e+00, -1.407823e+00, -1.410050e+00, -1.412289e+00, -1.414539e+00,
        -1.417937e+00, -1.420216e+00, -1.423659e+00, -1.427128e+00, -1.431798e+00, -1.436519e+00, -1.441291e+00,
        -1.447332e+00, -1.454693e+00, -1.460924e+00, -1.468521e+00, -1.477556e+00, -1.485452e+00, -1.494850e+00,
        -1.503070e+00, -1.512862e+00, -1.521434e+00, -1.531653e+00, -1.540608e+00, -1.551294e+00, -1.560667e+00,
        -1.571865e+00, -1.583359e+00, -1.595166e+00, -1.607303e+00, -1.621602e+00, -1.634512e+00, -1.647817e+00,
        -1.661544e+00, -1.673664e+00, -1.688246e+00, -1.701147e+00, -1.712198e+00, -1.725842e+00, -1.737549e+00,
        -1.749580e+00, -1.761954e+00, -1.772113e+00, -1.785156e+00, -1.798603e+00, -1.809668e+00, -1.821023e+00,
        -1.835647e+00, -1.847712e+00, -1.860121e+00, -1.872895e+00, -1.886057e+00, -1.896196e+00, -1.910095e+00,
        -1.920819e+00, -1.931814e+00, -1.939302e+00, -1.950782e+00, -1.958607e+00, -1.966576e+00, -1.974694e+00,
        -1.982967e+00, -1.991400e+00, -1.995679e+00, -2.004365e+00, -2.011441e+00, -2.017729e+00, -2.024109e+00,
        -2.030584e+00, -2.036212e+00, -2.041914e+00, -2.047208e+00, -2.052566e+00, -2.057496e+00, -2.061981e+00,
        -2.066513e+00, -2.070581e+00, -2.074688e+00, -2.078314e+00, -2.082494e+00, -2.085657e+00, -2.089376e+00,
        -2.092589e+00, -2.095826e+00, -2.098542e+00, -2.101275e+00, -2.104025e+00, -2.106793e+00, -2.109579e+00,
        -2.111821e+00, -2.114074e+00, -2.115771e+00, -2.118045e+00, -2.119186e+00, -2.120904e+00, -2.122053e+00,
        -2.122629e+00, -2.123205e+00, -2.123205e+00, -2.122629e+00, -2.122053e+00, -2.120331e+00, -2.118615e+00,
        -2.115205e+00, -2.111821e+00, -2.107905e+00, -2.102923e+00, -2.097997e+00, -2.093126e+00, -2.087247e+00,
        -2.081970e+00, -2.076238e+00, -2.070581e+00, -2.064997e+00, -2.059982e+00, -2.054531e+00, -2.049635e+00,
        -2.044793e+00, -2.040005e+00, -2.035269e+00, -2.030584e+00, -2.025949e+00, -2.021363e+00, -2.016825e+00,
        -2.011887e+00, -2.007446e+00, -2.002614e+00, -2.000000e+00, -1.991400e+00, -1.987163e+00, -1.982967e+00,
        -1.978811e+00, -1.974694e+00, -1.970616e+00, -1.962574e+00, -1.958607e+00, -1.954677e+00, -1.950782e+00,
        -1.943095e+00, -1.939302e+00, -1.935542e+00, -1.928118e+00, -1.924453e+00, -1.917215e+00, -1.913640e+00,
        -1.906578e+00, -1.903090e+00, -1.896196e+00, -1.892790e+00, -1.886057e+00, -1.882729e+00, -1.876148e+00,
        -1.872895e+00, -1.866461e+00, -1.860121e+00, -1.856985e+00, -1.850781e+00, -1.844664e+00, -1.841638e+00,
        -1.835647e+00, -1.829738e+00, -1.823909e+00, -1.821023e+00, -1.815309e+00, -1.809668e+00, -1.804100e+00,
        -1.798603e+00, -1.793174e+00, -1.790485e+00, -1.785156e+00, -1.779892e+00, -1.774691e+00, -1.769551e+00,
        -1.764472e+00, -1.759451e+00, -1.754487e+00, -1.749580e+00, -1.742321e+00, -1.737549e+00, -1.732828e+00,
        -1.728158e+00, -1.723538e+00, -1.718967e+00, -1.712198e+00, -1.707744e+00, -1.703335e+00, -1.698970e+00,
        -1.692504e+00, -1.688246e+00, -1.684030e+00, -1.677781e+00, -1.673664e+00, -1.667562e+00, -1.663540e+00,
        -1.657577e+00, -1.653647e+00, -1.647817e+00, -1.643974e+00, -1.638272e+00, -1.632644e+00, -1.628932e+00,
        -1.623423e+00, -1.617983e+00, -1.612610e+00, -1.609065e+00, -1.603801e+00, -1.598599e+00, -1.593460e+00,
        -1.588380e+00, -1.583359e+00, -1.578396e+00, -1.573489e+00, -1.568636e+00, -1.563837e+00, -1.559091e+00,
        -1.554396e+00, -1.548214e+00, -1.543634e+00, -1.539102e+00, -1.534617e+00, -1.528708e+00, -1.524329e+00,
        -1.518557e+00, -1.514279e+00, -1.508638e+00, -1.504456e+00, -1.498941e+00, -1.494850e+00, -1.489455e+00,
        -1.484126e+00, -1.480172e+00, -1.474955e+00, -1.469800e+00, -1.464706e+00, -1.459671e+00, -1.454693e+00,
        -1.449772e+00, -1.444906e+00, -1.440093e+00, -1.435334e+00, -1.430626e+00, -1.424812e+00, -1.420216e+00,
        -1.415669e+00, -1.410050e+00, -1.405607e+00, -1.400117e+00, -1.395774e+00, -1.390406e+00, -1.385103e+00,
        -1.380907e+00, -1.375718e+00, -1.370590e+00, -1.365523e+00, -1.360514e+00, -1.355561e+00, -1.350665e+00,
        -1.345823e+00, -1.340084e+00, -1.335358e+00, -1.330683e+00, -1.325139e+00, -1.320572e+00, -1.315155e+00,
        -1.310691e+00, -1.305395e+00, -1.300162e+00, -1.295849e+00, -1.290730e+00, -1.285670e+00, -1.280669e+00,
        -1.274905e+00, -1.270026e+00, -1.265200e+00, -1.260428e+00, -1.254925e+00, -1.250264e+00, -1.244888e+00,
        -1.240332e+00, -1.235077e+00, -1.229885e+00, -1.224754e+00, -1.219683e+00, -1.214670e+00, -1.209715e+00,
        -1.204815e+00, -1.199283e+00, -1.194499e+00, -1.189767e+00, -1.184422e+00, -1.179142e+00, -1.174574e+00,
        -1.169411e+00, -1.164309e+00, -1.159267e+00, -1.154282e+00, -1.148742e+00, -1.143876e+00, -1.139063e+00,
        -1.133713e+00, -1.129011e+00, -1.123782e+00, -1.118615e+00, -1.113509e+00, -1.108463e+00, -1.103474e+00,
        -1.098542e+00, -1.093665e+00, -1.088310e+00, -1.083546e+00, -1.078314e+00, -1.073143e+00, -1.068542e+00,
        -1.063486e+00, -1.058489e+00, -1.053057e+00, -1.048177e+00, -1.043351e+00, -1.038105e+00, -1.033389e+00,
        -1.028260e+00, -1.023192e+00, -1.018181e+00, -1.013228e+00, -1.008331e+00, -1.003051e+00, -1.000000e+00,
        -9.913998e-01, -9.871628e-01, -9.829667e-01, -9.788107e-01, -9.746941e-01, -9.665762e-01, -9.625735e-01,
        -9.586073e-01, -9.546770e-01, -9.469216e-01, -9.430951e-01, -9.393022e-01, -9.318141e-01, -9.281180e-01,
        -9.244530e-01, -9.172146e-01, -9.136402e-01, -9.100949e-01, -9.030900e-01, -8.996295e-01, -8.927900e-01,
        -8.894103e-01, -8.860566e-01, -8.794261e-01, -8.761484e-01, -8.696662e-01, -8.632794e-01, -8.601209e-01,
        -8.538720e-01};

    const std::vector<double> sg_log_sigma = {
        4.734800e+00, 4.742725e+00, 4.751279e+00, 4.758912e+00, 4.767156e+00, 4.775974e+00, 4.784617e+00, 4.793092e+00,
        4.802089e+00, 4.811575e+00, 4.820201e+00, 4.829947e+00, 4.838849e+00, 4.848805e+00, 4.857935e+00, 4.868056e+00,
        4.877947e+00, 4.887617e+00, 4.897627e+00, 4.907949e+00, 4.918030e+00, 4.928396e+00, 4.939020e+00, 4.949878e+00,
        4.960471e+00, 4.971276e+00, 4.981819e+00, 4.992995e+00, 5.004321e+00, 5.017033e+00, 5.025306e+00, 5.037426e+00,
        5.049218e+00, 5.060698e+00, 5.071882e+00, 5.082785e+00, 5.093422e+00, 5.107210e+00, 5.117271e+00, 5.130334e+00,
        5.143015e+00, 5.155336e+00, 5.167317e+00, 5.178977e+00, 5.190332e+00, 5.201397e+00, 5.214844e+00, 5.227887e+00,
        5.240549e+00, 5.252853e+00, 5.264818e+00, 5.276462e+00, 5.290035e+00, 5.301030e+00, 5.313867e+00, 5.326336e+00,
        5.340444e+00, 5.352183e+00, 5.365488e+00, 5.378398e+00, 5.390935e+00, 5.403121e+00, 5.416641e+00, 5.429752e+00,
        5.442480e+00, 5.454845e+00, 5.468347e+00, 5.481443e+00, 5.494155e+00, 5.506505e+00, 5.519828e+00, 5.532754e+00,
        5.546543e+00, 5.559907e+00, 5.572872e+00, 5.585461e+00, 5.598791e+00, 5.611723e+00, 5.625312e+00, 5.638489e+00,
        5.651278e+00, 5.663701e+00, 5.676694e+00, 5.690196e+00, 5.703291e+00, 5.716003e+00, 5.728354e+00, 5.741152e+00,
        5.754348e+00, 5.766413e+00, 5.778874e+00, 5.791691e+00, 5.804139e+00, 5.816241e+00, 5.828660e+00, 5.840733e+00,
        5.852480e+00, 5.864511e+00, 5.876218e+00, 5.887617e+00, 5.899273e+00, 5.910624e+00, 5.921686e+00, 5.932981e+00,
        5.943989e+00, 5.954725e+00, 5.965202e+00, 5.975432e+00, 5.984977e+00, 5.994757e+00, 6.004321e+00, 6.012837e+00,
        6.021189e+00, 6.029384e+00, 6.037426e+00, 6.041393e+00, 6.049218e+00, 6.053078e+00, 6.056905e+00, 6.060698e+00,
        6.064458e+00, 6.068186e+00, 6.068186e+00, 6.068186e+00, 6.068186e+00, 6.064458e+00, 6.060698e+00, 6.056905e+00,
        6.049218e+00, 6.041393e+00, 6.033424e+00, 6.021189e+00, 6.008600e+00, 5.993436e+00, 5.979093e+00, 5.965672e+00,
        5.952792e+00, 5.941511e+00, 5.932474e+00, 5.924796e+00, 5.918555e+00, 5.912753e+00, 5.907949e+00, 5.902547e+00,
        5.897077e+00, 5.890980e+00, 5.885361e+00, 5.879096e+00, 5.873902e+00, 5.869232e+00, 5.865104e+00, 5.862131e+00,
        5.860338e+00, 5.860338e+00, 5.862131e+00, 5.865104e+00, 5.869232e+00, 5.874482e+00, 5.880814e+00, 5.888179e+00,
        5.895975e+00, 5.904716e+00, 5.914343e+00, 5.924279e+00, 5.934498e+00, 5.944976e+00, 5.956649e+00, 5.968483e+00,
        5.980912e+00, 5.994317e+00, 6.008600e+00, 6.025306e+00, 6.037426e+00, 6.056905e+00, 6.071882e+00, 6.089905e+00,
        6.110590e+00, 6.130334e+00, 6.149219e+00, 6.170262e+00, 6.193125e+00, 6.214844e+00, 6.238046e+00, 6.260071e+00,
        6.283301e+00, 6.305351e+00, 6.326336e+00, 6.348305e+00, 6.369216e+00, 6.387390e+00, 6.406540e+00, 6.426511e+00,
        6.444045e+00, 6.460898e+00, 6.478566e+00, 6.495544e+00, 6.511883e+00, 6.528917e+00, 6.545307e+00, 6.562293e+00,
        6.578639e+00, 6.595496e+00, 6.611723e+00, 6.627366e+00, 6.642465e+00, 6.657056e+00, 6.671173e+00, 6.684845e+00,
        6.697229e+00, 6.709270e+00, 6.720159e+00, 6.730782e+00, 6.741152e+00, 6.750508e+00, 6.758912e+00, 6.767156e+00,
        6.775246e+00, 6.782473e+00, 6.790285e+00, 6.796574e+00, 6.803457e+00, 6.810233e+00, 6.816241e+00, 6.822168e+00,
        6.828015e+00, 6.833147e+00, 6.837588e+00, 6.841985e+00, 6.845098e+00, 6.847573e+00, 6.849419e+00, 6.850033e+00,
        6.850033e+00, 6.849419e+00, 6.848189e+00, 6.846337e+00, 6.844477e+00, 6.842609e+00, 6.840106e+00, 6.838219e+00,
        6.836324e+00, 6.835056e+00, 6.834421e+00, 6.833784e+00, 6.833147e+00, 6.832509e+00, 6.831230e+00, 6.829947e+00,
        6.828015e+00, 6.825426e+00, 6.821514e+00, 6.816241e+00, 6.810233e+00, 6.802089e+00, 6.793092e+00, 6.783189e+00,
        6.771587e+00, 6.759668e+00, 6.745855e+00, 6.731589e+00, 6.716838e+00, 6.700704e+00, 6.684845e+00, 6.668386e+00,
        6.650308e+00, 6.633468e+00, 6.614897e+00, 6.596597e+00, 6.578639e+00, 6.559907e+00, 6.541579e+00, 6.523746e+00,
        6.506505e+00, 6.488551e+00, 6.471292e+00, 6.453318e+00, 6.437751e+00, 6.419956e+00, 6.404834e+00, 6.389166e+00,
        6.372912e+00, 6.357935e+00, 6.342423e+00, 6.326336e+00, 6.311754e+00, 6.296665e+00, 6.281033e+00, 6.267172e+00,
        6.252853e+00, 6.238046e+00, 6.225309e+00, 6.209515e+00, 6.195900e+00, 6.181844e+00, 6.170262e+00, 6.155336e+00,
        6.143015e+00, 6.130334e+00, 6.113943e+00, 6.100371e+00, 6.089905e+00, 6.075547e+00, 6.060698e+00, 6.049218e+00,
        6.033424e+00, 6.021189e+00, 6.008600e+00, 5.994317e+00, 5.980912e+00, 5.967080e+00, 5.953276e+00, 5.939519e+00,
        5.925312e+00, 5.911158e+00, 5.897077e+00, 5.883093e+00, 5.868644e+00, 5.854306e+00, 5.839478e+00, 5.825426e+00,
        5.810233e+00, 5.795880e+00, 5.781037e+00, 5.766413e+00, 5.751279e+00, 5.736397e+00, 5.720986e+00, 5.705864e+00,
        5.691081e+00, 5.675778e+00, 5.659916e+00, 5.645422e+00, 5.629410e+00, 5.613842e+00, 5.598791e+00, 5.583199e+00,
        5.568202e+00, 5.552668e+00, 5.536558e+00, 5.521138e+00, 5.505150e+00, 5.489958e+00, 5.474216e+00, 5.457882e+00,
        5.442480e+00, 5.426511e+00, 5.411620e+00, 5.396199e+00, 5.380211e+00, 5.363612e+00, 5.348305e+00, 5.332438e+00,
        5.318063e+00, 5.301030e+00, 5.285557e+00, 5.269513e+00, 5.255273e+00, 5.238046e+00, 5.222716e+00, 5.206826e+00,
        5.193125e+00, 5.176091e+00, 5.161368e+00, 5.146128e+00, 5.130334e+00, 5.113943e+00, 5.100371e+00, 5.086360e+00,
        5.068186e+00, 5.053078e+00, 5.037426e+00, 5.025306e+00, 5.008600e+00, 4.993877e+00, 4.978637e+00, 4.963788e+00,
        4.948902e+00, 4.933993e+00, 4.918555e+00, 4.903633e+00, 4.888741e+00, 4.873902e+00, 4.859138e+00, 4.843855e+00,
        4.829304e+00, 4.814248e+00, 4.799341e+00, 4.784617e+00, 4.769377e+00, 4.755112e+00, 4.740363e+00, 4.725095e+00,
        4.710117e+00, 4.695482e+00, 4.680336e+00, 4.665581e+00, 4.651278e+00, 4.636488e+00, 4.621176e+00, 4.606381e+00,
        4.592177e+00, 4.577492e+00, 4.562293e+00, 4.547775e+00, 4.532754e+00, 4.517196e+00, 4.502427e+00, 4.487138e+00,
        4.472756e+00, 4.457882e+00, 4.442480e+00, 4.428135e+00, 4.413300e+00, 4.397940e+00, 4.383815e+00, 4.367356e+00,
        4.354108e+00, 4.338456e+00, 4.322219e+00, 4.307496e+00, 4.292256e+00, 4.278754e+00, 4.262451e+00, 4.247973e+00,
        4.232996e+00, 4.217484e+00, 4.201397e+00, 4.187521e+00, 4.173186e+00, 4.155336e+00, 4.139879e+00, 4.127105e+00,
        4.110590e+00, 4.096910e+00, 4.079181e+00, 4.064458e+00, 4.049218e+00, 4.033424e+00, 4.021189e+00, 4.004321e+00,
        3.989450e+00, 3.974051e+00, 3.959041e+00, 3.943989e+00, 3.928908e+00, 3.913814e+00, 3.898176e+00, 3.883093e+00,
        3.868056e+00, 3.853090e+00, 3.837588e+00, 3.822822e+00, 3.807535e+00, 3.792392e+00, 3.777427e+00, 3.762679e+00,
        3.747412e+00, 3.732394e+00, 3.717671e+00, 3.702431e+00, 3.687529e+00, 3.673021e+00, 3.658011e+00, 3.642465e+00,
        3.628389e+00, 3.612784e+00, 3.598791e+00, 3.583199e+00, 3.568202e+00, 3.553883e+00, 3.539076e+00, 3.525045e+00,
        3.509203e+00, 3.495544e+00, 3.480007e+00, 3.465383e+00, 3.451786e+00, 3.436163e+00, 3.421604e+00, 3.408240e+00,
        3.392697e+00, 3.378398e+00, 3.363612e+00, 3.348305e+00, 3.334454e+00, 3.320146e+00, 3.305351e+00, 3.290035e+00,
        3.276462e+00, 3.262451e+00, 3.247973e+00, 3.232996e+00, 3.217484e+00, 3.204120e+00, 3.190332e+00, 3.176091e+00,
        3.161368e+00, 3.146128e+00, 3.130334e+00, 3.117271e+00, 3.103804e+00, 3.089905e+00, 3.075547e+00, 3.060698e+00,
        3.045323e+00};

    const std::vector<double> sc_log_r = {
        7.520484e-01, 7.619278e-01, 7.723217e-01, 7.817554e-01, 7.916906e-01, 8.020893e-01, 8.122447e-01, 8.221681e-01,
        8.318698e-01, 8.419848e-01, 8.518696e-01, 8.621314e-01, 8.721563e-01, 8.819550e-01, 8.920946e-01, 9.020029e-01,
        9.122221e-01, 9.222063e-01, 9.319661e-01, 9.420081e-01, 9.518230e-01, 9.618955e-01, 9.722028e-01, 9.818186e-01,
        9.921115e-01, 1.000000e+00, 1.012837e+00, 1.021189e+00, 1.033424e+00, 1.041393e+00, 1.053078e+00, 1.060698e+00,
        1.071882e+00, 1.082785e+00, 1.093422e+00, 1.100371e+00, 1.110590e+00, 1.120574e+00, 1.133539e+00, 1.143015e+00,
        1.152288e+00, 1.161368e+00, 1.173186e+00, 1.181844e+00, 1.193125e+00, 1.201397e+00, 1.212188e+00, 1.222716e+00,
        1.232996e+00, 1.243038e+00, 1.252853e+00, 1.262451e+00, 1.271842e+00, 1.281033e+00, 1.292256e+00, 1.301030e+00,
        1.311754e+00, 1.322219e+00, 1.332438e+00, 1.342423e+00, 1.352183e+00, 1.361728e+00, 1.372912e+00, 1.382017e+00,
        1.392697e+00, 1.401401e+00, 1.411620e+00, 1.421604e+00, 1.431364e+00, 1.442480e+00, 1.451786e+00, 1.462398e+00,
        1.472756e+00, 1.481443e+00, 1.491362e+00, 1.502427e+00, 1.511883e+00, 1.522444e+00, 1.531479e+00, 1.541579e+00,
        1.551450e+00, 1.562293e+00, 1.571709e+00, 1.582063e+00, 1.592177e+00, 1.602060e+00, 1.611723e+00, 1.622214e+00,
        1.632457e+00, 1.642465e+00, 1.652246e+00, 1.661813e+00, 1.672098e+00, 1.682145e+00, 1.691965e+00, 1.702431e+00,
        1.711807e+00, 1.721811e+00, 1.732394e+00, 1.741939e+00, 1.752048e+00, 1.761928e+00, 1.772322e+00, 1.781755e+00,
        1.791691e+00, 1.802089e+00, 1.812245e+00, 1.822168e+00, 1.831870e+00, 1.841985e+00, 1.851870e+00, 1.862131e+00,
        1.872156e+00, 1.881955e+00, 1.892095e+00, 1.902003e+00, 1.912222e+00, 1.922206e+00, 1.931966e+00, 1.942008e+00,
        1.951823e+00, 1.961895e+00, 1.972203e+00, 1.981819e+00, 1.992111e+00, 2.000000e+00, 2.012837e+00, 2.021189e+00,
        2.033424e+00, 2.041393e+00, 2.053078e+00, 2.060698e+00, 2.071882e+00, 2.082785e+00, 2.093422e+00, 2.100371e+00,
        2.110590e+00, 2.120574e+00, 2.133539e+00, 2.143015e+00, 2.152288e+00, 2.161368e+00, 2.173186e+00, 2.181844e+00,
        2.193125e+00, 2.201397e+00, 2.212188e+00, 2.222716e+00, 2.232996e+00, 2.243038e+00, 2.252853e+00, 2.262451e+00,
        2.271842e+00, 2.281033e+00, 2.292256e+00, 2.301030e+00, 2.311754e+00, 2.322219e+00, 2.332438e+00, 2.342423e+00,
        2.352183e+00, 2.361728e+00, 2.372912e+00, 2.382017e+00, 2.392697e+00, 2.401401e+00, 2.411620e+00, 2.421604e+00,
        2.431364e+00, 2.442480e+00, 2.451786e+00, 2.462398e+00, 2.472756e+00, 2.481443e+00, 2.491362e+00, 2.502427e+00,
        2.511883e+00, 2.522444e+00, 2.531479e+00, 2.541579e+00, 2.551450e+00, 2.562293e+00, 2.571709e+00, 2.582063e+00,
        2.592177e+00, 2.602060e+00, 2.611723e+00, 2.622214e+00, 2.632457e+00, 2.642465e+00, 2.652246e+00, 2.661813e+00,
        2.672098e+00, 2.682145e+00, 2.691965e+00, 2.702431e+00, 2.711807e+00, 2.721811e+00, 2.732394e+00, 2.741939e+00,
        2.752048e+00, 2.761928e+00, 2.772322e+00, 2.781755e+00, 2.791691e+00, 2.802089e+00, 2.812245e+00, 2.822168e+00,
        2.831870e+00, 2.841985e+00, 2.851870e+00, 2.862131e+00, 2.872156e+00, 2.881955e+00, 2.892095e+00, 2.902003e+00,
        2.912222e+00, 2.922206e+00, 2.931966e+00, 2.942008e+00, 2.951823e+00, 2.961895e+00, 2.972203e+00, 2.981819e+00,
        2.992111e+00, 3.000000e+00, 3.012837e+00, 3.021189e+00, 3.033424e+00, 3.041393e+00, 3.053078e+00, 3.060698e+00,
        3.071882e+00, 3.082785e+00, 3.093422e+00, 3.100371e+00, 3.110590e+00, 3.120574e+00, 3.133539e+00, 3.143015e+00,
        3.152288e+00, 3.161368e+00, 3.173186e+00, 3.181844e+00, 3.193125e+00, 3.201397e+00, 3.212188e+00, 3.222716e+00,
        3.232996e+00, 3.243038e+00, 3.252853e+00, 3.262451e+00, 3.271842e+00, 3.281033e+00, 3.292256e+00, 3.301030e+00,
        3.311754e+00, 3.322219e+00, 3.332438e+00, 3.342423e+00, 3.352183e+00, 3.361728e+00, 3.372912e+00, 3.382017e+00,
        3.392697e+00, 3.401401e+00, 3.411620e+00, 3.421604e+00, 3.431364e+00, 3.442480e+00, 3.451786e+00, 3.462398e+00,
        3.472756e+00, 3.481443e+00, 3.491362e+00, 3.502427e+00, 3.511883e+00, 3.522444e+00, 3.531479e+00, 3.541579e+00,
        3.551450e+00, 3.562293e+00, 3.571709e+00, 3.582063e+00, 3.592177e+00, 3.602060e+00, 3.611723e+00, 3.622214e+00,
        3.632457e+00, 3.642465e+00, 3.652246e+00, 3.661813e+00, 3.672098e+00, 3.682145e+00, 3.691965e+00, 3.702431e+00,
        3.711807e+00, 3.721811e+00, 3.732394e+00, 3.741939e+00, 3.752048e+00, 3.761928e+00, 3.772322e+00, 3.781755e+00,
        3.791691e+00, 3.802089e+00, 3.812245e+00, 3.822168e+00, 3.831870e+00, 3.841985e+00, 3.851870e+00, 3.862131e+00,
        3.872156e+00, 3.881955e+00, 3.892095e+00, 3.902003e+00, 3.912222e+00, 3.922206e+00, 3.931966e+00, 3.942008e+00,
        3.951823e+00, 3.961895e+00, 3.972203e+00, 3.981819e+00, 3.992111e+00, 4.000000e+00, 4.012837e+00, 4.021189e+00,
        4.033424e+00, 4.041393e+00, 4.053078e+00, 4.060698e+00, 4.071882e+00, 4.082785e+00, 4.093422e+00, 4.100371e+00,
        4.110590e+00, 4.120574e+00, 4.133539e+00, 4.143015e+00, 4.152288e+00, 4.161368e+00, 4.173186e+00, 4.181844e+00,
        4.193125e+00, 4.201397e+00, 4.212188e+00, 4.222716e+00, 4.232996e+00, 4.243038e+00, 4.252853e+00, 4.262451e+00,
        4.271842e+00, 4.281033e+00, 4.292256e+00, 4.301030e+00, 4.311754e+00, 4.322219e+00, 4.332438e+00, 4.342423e+00,
        4.352183e+00, 4.361728e+00, 4.372912e+00, 4.382017e+00, 4.392697e+00, 4.401401e+00, 4.411620e+00, 4.421604e+00,
        4.431364e+00, 4.442480e+00, 4.451786e+00, 4.462398e+00, 4.472756e+00, 4.481443e+00, 4.491362e+00, 4.502427e+00,
        4.511883e+00, 4.522444e+00, 4.531479e+00, 4.541579e+00, 4.551450e+00, 4.562293e+00, 4.571709e+00, 4.582063e+00,
        4.592177e+00, 4.602060e+00, 4.611723e+00, 4.622214e+00, 4.632457e+00, 4.642465e+00, 4.652246e+00, 4.661813e+00,
        4.672098e+00, 4.682145e+00, 4.691965e+00, 4.702431e+00, 4.711807e+00, 4.721811e+00, 4.732394e+00, 4.741939e+00,
        4.752048e+00, 4.761928e+00, 4.772322e+00, 4.781755e+00, 4.791691e+00, 4.802089e+00, 4.812245e+00, 4.822168e+00,
        4.831870e+00, 4.841985e+00, 4.851870e+00, 4.862131e+00, 4.872156e+00, 4.881955e+00, 4.892095e+00, 4.902003e+00,
        4.912222e+00, 4.922206e+00, 4.931966e+00, 4.942008e+00, 4.951823e+00, 4.961895e+00, 4.972203e+00, 4.981819e+00,
        4.992111e+00, 5.000000e+00, 5.012837e+00, 5.021189e+00, 5.033424e+00, 5.041393e+00, 5.053078e+00, 5.060698e+00,
        5.071882e+00, 5.082785e+00, 5.093422e+00, 5.100371e+00, 5.110590e+00, 5.120574e+00, 5.133539e+00, 5.143015e+00,
        5.152288e+00, 5.161368e+00, 5.173186e+00, 5.181844e+00, 5.193125e+00, 5.201397e+00, 5.212188e+00, 5.222716e+00,
        5.232996e+00, 5.243038e+00, 5.252853e+00, 5.262451e+00, 5.271842e+00, 5.281033e+00, 5.292256e+00, 5.301030e+00,
        5.311754e+00, 5.322219e+00, 5.332438e+00, 5.342423e+00, 5.352183e+00, 5.361728e+00, 5.372912e+00, 5.382017e+00,
        5.392697e+00, 5.401401e+00, 5.411620e+00, 5.421604e+00, 5.431364e+00, 5.442480e+00, 5.451786e+00, 5.462398e+00,
        5.472756e+00, 5.481443e+00, 5.491362e+00, 5.502427e+00, 5.511883e+00, 5.522444e+00, 5.531479e+00, 5.541579e+00,
        5.551450e+00, 5.562293e+00, 5.571709e+00, 5.582063e+00, 5.592177e+00, 5.602060e+00, 5.611723e+00, 5.622214e+00,
        5.632457e+00, 5.642465e+00, 5.652246e+00, 5.661813e+00, 5.672098e+00, 5.682145e+00, 5.691965e+00, 5.702431e+00,
        5.711807e+00, 5.721811e+00, 5.732394e+00, 5.741939e+00, 5.752048e+00, 5.761928e+00, 5.772322e+00, 5.781755e+00,
        5.791691e+00};

    double const log_r_min = 0.7520484478194386;
    double const log_r_max = 5.791690649020118;
    class DiskCapture {
       public:
        constexpr static bool vel_dependent{true};
        // Type members
        template <typename Particles>
        static void add_acc_to(Particles const &particles, typename Particles::VectorArray &acceleration);
    };

    template <typename Vec>
    double disk_rho(Vec &r, double M) {
        const double Q = 1;
        const double alpha = 1;
        const double lambda = 1;
        auto M_dot = lambda * 0.22 * (M / 1e7) / (2 * consts::pi);
        auto R = sqrt(r.x * r.x + r.y * r.y);
        auto Omega = sqrt(M / (R * R * R));
        auto H = pow(Q / 2 / alpha * M_dot / M / Omega, 1.0 / 3) * R;
        auto rho0 = M / (2 * Q * consts::pi * R * R * R);
        return rho0 * exp(-(r.z * r.z) / (2 * H * H));
    }

    template <typename Vec>
    Vec disk_v(Vec &r, double M) {
        auto rr = sqrt(r.x * r.x + r.y * r.y);
        auto v = sqrt(M / rr);
        return Vec{-v * r.y / rr, v * r.x / rr, 0};
    }

    template <typename Particles>
    void DiskCapture::add_acc_to(const Particles &particles, typename Particles::VectorArray &acceleration) {
        size_t num = particles.number();
        auto const &p = particles.pos();
        auto const &v = particles.vel();
        auto const &m = particles.mass();
        auto const &r = particles.radius();

        for (size_t i = 1; i < num; ++i) {
            auto dr = p[i] - p[0];
            auto dv = v[i] - v[0];
            auto v_disk = disk_v(dr, m[0]);
            auto v_rel = dv - v_disk;
            auto rho = disk_rho(dr, m[0]);
            auto rd = r[i];
            auto vabs = sqrt(dot(v_rel, v_rel));
            /*double f1 = consts::pi * rd * rd * rho * dot(v_rel, v_rel);
            double f2 = 4 * consts::pi * consts::G * consts::G * m[i] * m[i] / dot(v_rel, v_rel) * rho;
            acceleration[i] -= std::max(f1, f2) * v_rel / vabs;
            acceleration[0] -= std::max(f1, f2) * v_rel / vabs;*/
            // double f1 = consts::pi * rd * rd * rho * dot(v_rel, v_rel);
            double f = 4 * consts::pi * consts::G * consts::G * m[i] * m[i] / dot(v_rel, v_rel) * rho;
            acceleration[i] -= f * v_rel / vabs / m[i];
            acceleration[0] += f * v_rel / vabs / m[0];
        }
    }

}  // namespace hub::force
