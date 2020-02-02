import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
from timeit import timeit


# Read wrapping probabilities from file and compute the
# convolution with the binomial distribution
# Returns R_L^h(p), R_L^e(p), R_L^b(p), R_L^1(p)
def conv_from_file(filename, p_values):

	with open( filename, 'r') as f:
	    lines = f.readlines()
	    yh = np.array([float(line.split()[1]) for line in lines])
	    yb = np.array([float(line.split()[2]) for line in lines])

	N = len(yh)

	# This is the clear bottleneck of this computation 
	# (optimize using cython??)
	RLh = np.array([ np.sum(binomial(N, p)*yh) for p in p_values]) 
	RLb = np.array([ np.sum(binomial(N, p)*yb) for p in p_values]) 

	return RLh, 2*RLh-RLb, RLb, RLh-RLb


# Computes the binomial distribution using the recursive definition 
def binomial(N, p):

	nmax = np.floor(N*p).astype(int)
	binom = np.zeros(N+1)
	binom[nmax+1] = 1.0

	for i in range(nmax,-1, -1):
		binom[i] = binom[i+1]*(i+1)*(1-p)/(N-i)/p

	for i in range(nmax, N+1):
		binom[i] = binom[i-1]*(N-i+1)*p/i/(1-p)
	
	binom = binom/np.sum(binom)
	return binom[1:]


# Find the index nearest of the array item that is nearest to the value  
def find_nearest(array, value):
    array = np.asarray(array)
    idx = (np.abs(array - value)).argmin()
    return idx


# Find the index of maximum in the array
def find_max(array):
    array = np.asarray(array)
    idx = (np.abs(array)).argmax()
    return idx


def main():	

	# General plot

	# 'Exact' values of the percolation threshold and 
	# the wrapping probabilities
	pc = 0.592746
	Rinf = [0.521058290, 0.690473725,  0.351642855, 0.169415435]
	
	pmin = pc-0.06
	pmax = pc+0.06
	pn = 301
	pp =  np.linspace(pmin,pmax,pn)

	# Output of the Newman-Ziff algorithm
	fnames1 = [ "perc32_1e9.dat","perc64_2e8.dat",
			    "perc128_5e7.dat", "perc256_1e7.dat"]
	
	fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2,  figsize=(10, 10)) 
	
	
	# Compute the wrapping probability 
	# with respect to the occupation probability 	
	for idx, fname in enumerate(fnames1): 
		
		RLh, RLe, RLb, RL1 = conv_from_file(fname, pp)

		ax1.plot(pp, RLh)
		ax2.plot(pp, RLe)
		ax3.plot(pp, RLb)
		ax4.plot(pp, RL1)

	# Draw the lines for R_L = R_inf(p_c) and p = p_c
	axes = [ax1, ax2, ax3, ax4]
	for R, ax in zip(Rinf, axes):
		ax.axhline(y = R, color = '0.6', linestyle = '--')
		ax.axvline(x = pc, color = '0.6', linestyle = '--')
 
	fig.text(0.5, 0.04, r'Occupation probability $p$', 
						ha='center')
	fig.text(0.04, 0.5, r'Wrapping probability $R_L(p)$', 
						va='center', rotation='vertical')
	fig.savefig("plots/plot1.png")


	# Finite-size scaling plot
	pmin = pc-0.00002
	pmax = pc+0.00004
	pn = 301
	pp =  np.linspace(pmin,pmax,pn)

	# Output of the Newman-Ziff algo
	fnames2 = [	"perc32_1e9.dat","perc64_2e8.dat", 
				"perc128_5e7.dat", "perc256_1e7.dat"]
	pcrit = []
	
	# Compute the wrapping probabilities
	for fname in fnames2: 

		RLh, RLe, RLb, RL1 = conv_from_file(fname, pp)
		
		# Find the p_c estimates
		pcrit.append([	pp[find_nearest(RLh, Rinf[0])],
						pp[find_nearest(RLe, Rinf[1])],
						pp[find_nearest(RLb, Rinf[2])],
						pp[find_max(RL1)]])
		

	pcrit = np.array(pcrit).T
	Lscale = (np.array([32, 64, 128, 256]))**(-11/4)
	colors = ['C0', 'C1', 'C2', 'C3']

	fig, ax = plt.subplots(figsize=(8, 6))

	# Fit a line to the p_c estimates scaling as L^{-11/4} 
	for plist, c in zip(pcrit, colors):
		coef, V = np.polyfit(Lscale, plist, 1, cov=True)

		print("Intercept: {} +/- {}".format(coef[1], np.sqrt(V[1][1])))

		poly = np.poly1d(coef)
		ax.plot(Lscale, poly(Lscale), c, linewidth=1)
		ax.plot(Lscale, plist, '+' + c, markersize=12)
	
	ax.axvline(x = 0, color = '0.6', linestyle = '--', linewidth=1)
	ax.set_ylabel(r'$p_c$')
	ax.set_xlabel(r'$L^{-11/4}$')
	fig.savefig("plots/plot2.png")
	plt.show()


	"""
	# Draw the lattice that was saved in a text file
	# Fill in data
	with open('sample.dat') as file:
		data = np.array([[float(digit) for digit in line.split()] for line in file])

	N = data.shape[0]

	# Make a figure + axes
	fig, ax = plt.subplots(1, 1, tight_layout=True)
	# Make color map
	my_cmap = ListedColormap(['w','r', 'k'])
	# Set the 'bad' values (nan) to be white and transparent
	my_cmap.set_bad(color='w', alpha=0)
	# Draw the boxes
	ax.imshow(data, interpolation='none', cmap=my_cmap, extent=[0, N, 0, N], zorder=0)
	# Turn off the axis labels
	ax.axis('off')
	plt.show()
	"""

print(timeit(main, number=1))

